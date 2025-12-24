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

		m_JobLock.Lock();
		for (unsigned int i = 0; i < numJobs; i++) {
			Job job;
			job.batchIdex = i;
			job.jobFnc = jobFnc;
			job.status = JobStatus::Waiting;
			memcpy(job.customParameter, parameter, parameterSize);

			job.counter = counter;
			switch (priority) {
				case JobPriority::High: {
					m_HighQueue.push(job);
					break;
				}
				case JobPriority::Medium: {
					m_MediumQueue.push(job);
					break;
				}
				case JobPriority::Low: {
					m_LowQueue.push(job);
					break;
				}
			}
		}

		m_JobLock.Unlock();

		return counter;
	}

	bool JobManager::GetNextJob(Job* outJob)
	{
		Job resJob = Job{};

		bool foundJob = false;
		
		m_JobLock.Lock();

		// Pop from sleep list
		for (size_t i = 0; i < m_WaitingJobs.size(); i++) {
			WaitingJob waitingJob = m_WaitingJobs[i];
			if (waitingJob.counter->GetCounter() <= 0) {
				m_WaitingJobs.erase(m_WaitingJobs.begin() + i);
				resJob = waitingJob.job;
				foundJob = true;
				break;
			}
		}

		if (!m_HighQueue.empty()) {
			resJob = m_HighQueue.front();
			m_HighQueue.pop();
			foundJob = true;
		}

		if (!m_MediumQueue.empty()) {
			resJob = m_MediumQueue.front();
			m_MediumQueue.pop();
			foundJob = true;
		}

		if (!m_LowQueue.empty()) {
			resJob = m_LowQueue.front();
			m_LowQueue.pop();
			foundJob = true;
		}

		m_JobLock.Unlock();

		if (foundJob) {
			resJob.status = JobStatus::Running;
			*outJob = resJob;
		}

		return foundJob;
	}

	void JobManager::PauseJob(Job job, Ref<JobBatchCounter> counter)
	{
		WaitingJob waitingJob;
		waitingJob.counter = counter;
		waitingJob.job = job;

		m_JobLock.Lock();
		m_WaitingJobs.push_back(waitingJob);
		m_JobLock.Unlock();
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
			if (!instance->GetNextJob(&g_CurJob)) {
				continue;
			}

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

