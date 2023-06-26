#include "gepch.h"
#include "Timer.h"
using namespace std::chrono;

namespace ProjectGE {
	Timer::Timer() {
		m_Last = steady_clock::now();
		m_TotalTime = 0;
	}

	float Timer::Mark() {
		const auto old = m_Last;
		m_Last = steady_clock::now();
		const duration<float> frameTime = m_Last - old;
		m_DeltaTime = frameTime.count();
		m_TotalTime += m_DeltaTime;
		return m_DeltaTime;
	}

	float Timer::Peek() const {
		return duration<float>(steady_clock::now() - m_Last).count();
	}

	float Timer::GetTotalTime() const
	{
		return m_TotalTime;
	}

	float Timer::GetDeltaTime() const
	{
		return m_DeltaTime;
	}
};

