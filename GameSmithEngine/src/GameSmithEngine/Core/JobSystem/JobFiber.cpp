#include "gepch.h"
#include "JobFiber.h"
#include "GameSmithEngine/Core/Log.h"

#define FIBER_STACK_SIZE 1000

namespace GameSmith {
	JobFiber JobFiber::CreateJobFiber(void(*jobFnc)(void*), void* parm)
	{
		// Calling Fiber will own the lifetime of this pointer and to free it
		FiberParameters* fParm = new FiberParameters;
		fParm->jobFnc = jobFnc;
		fParm->parm = parm;

		void* osFiber = nullptr;
#ifdef GE_PLATFORM_WINDOWS
		osFiber = CreateFiber(FIBER_STACK_SIZE, (LPFIBER_START_ROUTINE)JobFiberEntryPoint, fParm);
#endif

		return JobFiber(osFiber);
	}

	JobFiber JobFiber::ConvertThreadToJobFiber()
	{
		void* osFiber = nullptr;

#ifdef GE_PLATFORM_WINDOWS
		osFiber = ConvertThreadToFiber(nullptr);
#endif
		return JobFiber(osFiber);
	}

	void JobFiber::DeleteJobFiber(JobFiber fiber)
	{
		GE_CORE_ASSERT(!fiber.isEmpty(), "Fiber OS pointer unexpectedly null");
#ifdef GE_PLATFORM_WINDOWS
		DeleteFiber(fiber.m_OSFiber);
#endif
	}

	void JobFiber::LoadJobFiber(JobFiber newFiber)
	{
		GE_CORE_ASSERT(!newFiber.isEmpty(), "Fiber OS pointer unexpectedly null");
#ifdef GE_PLATFORM_WINDOWS
		SwitchToFiber(newFiber.m_OSFiber);
#endif
	}

	void JobFiberEntryPoint()
	{
		FiberParameters* heapFParm = nullptr;

#ifdef GE_PLATFORM_WINDOWS
		heapFParm = static_cast<FiberParameters*>(GetFiberData());
#endif

		FiberParameters stackFParm = *heapFParm;
		delete heapFParm;

		stackFParm.jobFnc(stackFParm.parm);
	}
};

