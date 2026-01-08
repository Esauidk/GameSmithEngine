#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "JobFiber.h"
#include <atomic>
#include <queue>
#include <thread>

#include "GameSmithEngine/Utilities/Locks/Spinlock.h"

#define WORKER_THREAD_COUNT 6 // TODO: Make worker thread count static for now, come back and make it configurable
#define JOB_MAX_PARAMETER_SIZE 1000

namespace GameSmith {
	enum class JobPriority {
		High,
		Medium,
		Low
	};

	enum class JobStatus {
		Waiting,
		Running
	};

	class JobBatchCounter {
		friend class JobManager;
		friend void WorkerThreadFunction();
	public:
		unsigned int GetCounter();
	private:
		JobBatchCounter(unsigned int batchSize);
		void Decrement();
	private:
		std::atomic<unsigned int> m_Count;
	};

	// The main function workers use when idling for jobs
	void WorkerThreadFunction();

	struct Job {
		unsigned int batchIdex = 0;
		void (*jobFnc)(JobStandardParamters, void*) = nullptr;
		char customParameter[JOB_MAX_PARAMETER_SIZE] = {};
		JobStatus status = JobStatus::Waiting;
		Ref<JobBatchCounter> counter;

		JobFiber runningFiber = JobFiber::CreateEmptyJobFiber();
	};

	class GE_API JobManager {
	// Allow the fiber workers to access private functions
	friend void WorkerThreadFunction();
	friend void GE_API WorkerPauseCurrentJob(Ref<JobBatchCounter> marker);

	public:
		static void Init();
		static void Shutdown();
		Ref<JobBatchCounter> StartJobs(
			void (*jobFnc)(JobStandardParamters, void*),
			void* parameter,
			unsigned int parameterSize,
			unsigned int numJobs,
			JobPriority priority
		);

		inline static JobManager* GetInstance() { return s_Instance; };
	private:
		JobManager();
		bool GetNextJob(Job* outJob);
		void PauseJob(Job job, Ref<JobBatchCounter>);
	private:
		struct WaitingJob {
			Ref<JobBatchCounter> counter;
			Job job;
		};
	private:
		static JobManager* s_Instance;
	private:
		std::thread m_Worker[WORKER_THREAD_COUNT];


		Spinlock m_JobLock;
		std::vector<WaitingJob> m_WaitingJobs;
		std::queue<Job> m_HighQueue;
		std::queue<Job> m_MediumQueue;
		std::queue<Job> m_LowQueue;
	};
};
