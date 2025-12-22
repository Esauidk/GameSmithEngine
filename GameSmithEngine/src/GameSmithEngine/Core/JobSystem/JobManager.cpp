#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "JobUtils.h"
#include "JobManager.h"

namespace GameSmith {
	thread_local JobFiber g_WorkerFiber = JobFiber::CreateEmptyJobFiber();
	thread_local Job g_CurJob = Job{};

	JobManager* JobManager::s_Instance = nullptr;


	JobManager::JobManager() {
		for (int i = 0; i < WORKER_THREAD_COUNT; i++) {
			m_Worker[i] = std::move(std::thread(WorkerThreadFunction));
		}
	}

	bool JobManager::JobsAvailable()
	{
		for (size_t i = 0; i < m_WaitingJobs.Size(); i++) {
			WaitingJob waitingJob = m_WaitingJobs.Get(i);
			if (waitingJob.counter->GetCounter() <= 0) {
				return true;
			}
		}

		return
			!m_HighQueue.IsEmpty() ||
			!m_MediumQueue.IsEmpty() ||
			!m_LowQueue.IsEmpty();
	}

	void JobManager::Init() {
		if (s_Instance == nullptr) {
			s_Instance = new JobManager();
		}
	}

	void JobManager::Shutdown() {
		if (s_Instance != nullptr) {
			delete s_Instance;
		}
	}

	Ref<JobBatchCounter> JobManager::StartJobs(
		void(*jobFnc)(JobStandardParamters, void*),
		void* parameter,
		unsigned int parameterSize,
		unsigned int numJobs,
		JobPriority priority)
	{
		GE_CORE_ASSERT(parameterSize <= JOB_MAX_PARAMETER_SIZE, "Parameter size is greater than the max supported size for jobs");

		Ref<JobBatchCounter> counter = Ref<JobBatchCounter>(new JobBatchCounter(numJobs));

		for (unsigned int i = 0; i < numJobs; i++) {
			Job job;
			job.batchIdex = i;
			job.jobFnc = jobFnc;
			job.status = JobStatus::Waiting;
			memcpy(job.customParameter, parameter, parameterSize);

			job.counter = counter;

			switch (priority) {
				case JobPriority::High: {
					m_HighQueue.Push(job);
					break;
				}
				case JobPriority::Medium: {
					m_MediumQueue.Push(job);
					break;
				}
				case JobPriority::Low: {
					m_LowQueue.Push(job);
					break;
				}
			}
		}

		return counter;
	}

	Job JobManager::GetNextJob()
	{
		Job resJob = Job{};

		// Pop from sleep list
		for (size_t i = 0; i < m_WaitingJobs.Size(); i++) {
			WaitingJob waitingJob = m_WaitingJobs.Get(i);
			if (waitingJob.counter->GetCounter() <= 0) {
				m_WaitingJobs.Remove(i);
				resJob = waitingJob.job;
				break;
			}
		}

		if (!m_HighQueue.IsEmpty()) resJob = m_HighQueue.Pop();

		if (!m_MediumQueue.IsEmpty()) resJob = m_MediumQueue.Pop();

		if (!m_LowQueue.IsEmpty()) resJob = m_LowQueue.Pop();

		resJob.status = JobStatus::Running;
		return resJob;
	}

	void JobManager::PauseJob(Job job, Ref<JobBatchCounter> counter)
	{
		WaitingJob waitingJob;
		waitingJob.counter = counter;
		waitingJob.job = job;

		m_WaitingJobs.PushBack(waitingJob);
	}

	JobBatchCounter::JobBatchCounter(unsigned int batchSize) : m_Count(batchSize){}

	void JobBatchCounter::Decrement()
	{
		m_Count--;
	}

	unsigned int JobBatchCounter::GetCounter()
	{
		return m_Count.load();
	}

	void WorkerThreadFunction()
	{
		GE_CORE_INFO("Starting Job Worker Thread");
		// Convert this worker thread into a fiber and save information about it in thread storage
		g_WorkerFiber = JobFiber::ConvertThreadToJobFiber();

		auto instance = JobManager::GetInstance();
		while (true) {
			if (!instance->JobsAvailable()) {
				GE_CORE_INFO("No jobs available, idling");
				continue;
			}

			g_CurJob = instance->GetNextJob();

			if (g_CurJob.runningFiber.isEmpty()) {
				JobStandardParamters jobParams;
				jobParams.batchIndex = g_CurJob.batchIdex;

				// Create a fiber on this thread for the job
				g_CurJob.runningFiber = JobFiber::CreateJobFiber(g_CurJob.jobFnc, jobParams, g_CurJob.customParameter);
			}

			// Switch over this thread to that fiber
			// We will come back to this worker loop fiber if 1) The job finished, 2) The job yielded
			JobFiber::LoadJobFiber(g_CurJob.runningFiber);

			if (!g_CurJob.runningFiber.isEmpty() && g_CurJob.status != JobStatus::Waiting) {
				// Clean up the fiber after it is done
				JobFiber::DeleteJobFiber(g_CurJob.runningFiber);

				// Decrement the counter related to the job
				g_CurJob.counter->Decrement();

				// Mark that we no longer have a job on this thread
				g_CurJob = Job{};
			}

			GE_CORE_INFO("Finished processing a job");
		}
	}

	void WorkerPauseCurrentJob(Ref<JobBatchCounter> marker)
	{
		GE_APP_ASSERT(!g_CurJob.runningFiber.isEmpty(), "This thread is not running a fiber job, there is nothing to pause");

		auto instance = JobManager::GetInstance();
		g_CurJob.status = JobStatus::Waiting;
		instance->PauseJob(g_CurJob, marker);
		JobFiber::LoadJobFiber(g_WorkerFiber);
	}

	void GE_API WorkerCompleteCurrentJob()
	{
		GE_APP_ASSERT(!g_CurJob.runningFiber.isEmpty(), "This thread is not running a fiber job, there is nothing to complete");
		JobFiber::LoadJobFiber(g_WorkerFiber);
	}
};

