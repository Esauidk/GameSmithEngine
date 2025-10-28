#pragma once

namespace GameSmith {
	struct FiberParameters {
		void (*jobFnc)(void*);
		void* parm;
	};

	class JobFiber {
	public:
		static JobFiber CreateJobFiber(void (*jobFnc)(void*), void* parm);
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

