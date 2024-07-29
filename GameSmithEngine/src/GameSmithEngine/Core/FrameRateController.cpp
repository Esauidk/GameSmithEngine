#include "gepch.h"
#include "FrameRateController.h"
#include "Application.h"
#include <thread>
#include "Log.h"

#pragma comment(lib, "winmm.lib")

namespace GameSmith {
	FrameRateController::FrameRateController() : m_FrameRate(60.f), m_LimitExists(true)
	{
	}

	void FrameRateController::CheckAndPerformDelay()
	{
		auto& timer = Application::Get().GetTimer();
		float limit = (1000.0f * (1.0f / m_FrameRate));
		auto diff = (1000.0f * (1.0f / m_FrameRate)) - (timer.MeasureWithoutMark() * 1000.0f);
		if (m_LimitExists && diff > 0) {
			static const UINT periodMin = []
				{
					TIMECAPS tc;
					timeGetDevCaps(&tc, sizeof(TIMECAPS));
					return tc.wPeriodMin;
				}();

			timeBeginPeriod(periodMin);
			::Sleep(static_cast<DWORD>(diff));
			timeEndPeriod(periodMin);
			float time = (1000.0f * (1.0f / m_FrameRate)) - (timer.MeasureWithoutMark() * 1000.0f);
			int i = 0;
			
		}
	}
};

