#pragma once

namespace GameSmith {
	struct JobStandardParamters {
		unsigned int batchIndex;
	};

	struct FiberParameters {
		void (*jobFnc)(JobStandardParamters, void*);
		JobStandardParamters jobParams;
		void* customParm;
	};

	class JobFiber {
	public:
		static JobFiber CreateJobFiber(void (*jobFnc)(JobStandardParamters, void*), JobStandardParamters jobParms, void* customParm);
		static JobFiber ConvertThreadToJobFiber();
		static void DeleteJobFiber(JobFiber fiber);
		static void LoadJobFiber(JobFiber newFiber);

		inline static JobFiber CreateEmptyJobFiber() { return JobFiber(nullptr); }

		bool isEmpty() { return m_OSFiber == nullptr; }
	private:
		inline JobFiber(void* osFiber) : m_OSFiber(osFiber) {}
	private:
		void* m_OSFiber;
	};

	void JobFiberEntryPoint();
};

