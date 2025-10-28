#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "JobManager.h"

namespace GameSmith {
	thread_local JobFiber g_WorkerFiber = JobFiber::CreateEmptyJobFiber();
	thread_local Job g_CurJob = Job{};

	JobManager* JobManager::s_Instance = nullptr;


	JobManager::JobManager() {
		for (int i = 0; i < WORKER_THREAD_COUNT; i++) {
			m_worker[i] = std::thread(WorkerThreadFunction);
		}
	}

	bool JobManager::JobsAvailable()
	{
		return false;
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
		void(*jobFnc)(void*),
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
			memcpy(job.parameter, parameter, parameterSize);

			job.counter = counter;
		}

		// TODO: Add to queue

		return counter;
	}

	Job JobManager::GetNextJob()
	{
		return Job();
	}

	JobManager* JobManager::GetInstance() {
		// TODO: Create instance
		return nullptr;
	}

	JobBatchCounter::JobBatchCounter(unsigned int batchSize)
	{
		m_Count.store(batchSize);
	}

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
				// Create a fiber on this thread for the job
				g_CurJob.runningFiber = JobFiber::CreateJobFiber(g_CurJob.jobFnc, g_CurJob.parameter);
			}

			// Switch over this thread to that fiber
			// We will come back to this worker loop fiber if 1) The job finished, 2) The job yielded
			JobFiber::LoadJobFiber(g_CurJob.runningFiber);

			if (!g_CurJob.runningFiber.isEmpty()) {
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
};

