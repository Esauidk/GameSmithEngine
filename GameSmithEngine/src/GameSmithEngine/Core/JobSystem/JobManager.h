#pragma once
#include "GameSmithEngine/Core/Core.h"
#include <queue>
#include <thread>
#include <atomic>
#include "JobFiber.h"

#define WORKER_THREAD_COUNT 6 // TODO: Make worker thread count static for now, come back and make it configurable
#define JOB_MAX_PARAMETER_SIZE 1000

namespace GameSmith {
	enum class JobPriority {
		High,
		Medium,
		Low
	};

	class JobBatchCounter {
	friend class JobManager;
	friend void WorkerThreadFunction();

	private:
		JobBatchCounter(unsigned int batchSize);
		void Decrement();
		unsigned int GetCounter();
	private:
		std::atomic<unsigned int> m_Count;
	};

	struct Job {
		unsigned int batchIdex;
		void (*jobFnc)(void*);
		char parameter[JOB_MAX_PARAMETER_SIZE];
		Ref<JobBatchCounter> counter;

		JobFiber runningFiber = JobFiber::CreateEmptyJobFiber();
	};

	class GE_API JobManager {
	friend void WorkerThreadFunction();
	public:
		static void Init();
		static void Shutdown();
		Ref<JobBatchCounter> StartJobs(
			void (*jobFnc)(void*),
			void* parameter,
			unsigned int parameterSize,
			unsigned int numJobs,
			JobPriority priority
		);

		static JobManager* GetInstance();
	private:
		JobManager();
		bool JobsAvailable();
		Job GetNextJob();
	private:
		static JobManager* s_Instance;
	private:
		std::thread m_worker[WORKER_THREAD_COUNT];

		std::queue<Job> m_HighQueue;
		std::queue<Job> m_MediumQueue;
		std::queue<Job> m_LowQueue;
	};

	void WorkerThreadFunction();
};
