#include "gepch.h"
#include "Timer.h"
using namespace std::chrono;

namespace GameSmith {
	Timer::Timer() : m_TotalTime(0), m_DeltaTime(0) {
		m_Last = steady_clock::now();
	}

	float Timer::Mark() {
		const auto old = m_Last;
		m_Last = steady_clock::now();
		const duration<float> frameTime = m_Last - old;
		m_DeltaTime = frameTime.count();
		m_TotalTime += m_DeltaTime;
		return m_DeltaTime;
	}

	void Timer::Reset()
	{
		m_Last = steady_clock::now();
		m_DeltaTime = 0;
		m_TotalTime = 0;
	}

	float Timer::GetTotalTimeSeconds() const
	{
		const duration<float> curDelta = steady_clock::now() - m_Last;
		return (m_TotalTime + curDelta.count());
	}

	float Timer::GetDeltaTimeSeconds() const
	{
		return m_DeltaTime;
	}
	float Timer::GetTotalTimeMilliseconds() const
	{
		const duration<float> curDelta = steady_clock::now() - m_Last;
		return (m_TotalTime + curDelta.count()) * 1000.0f;
	}
	float Timer::GetDeltaTimeMilliseconds() const
	{
		return m_DeltaTime * 1000.0f;
	}
};

