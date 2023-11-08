#pragma once
#include "gepch.h"

namespace GameSmith {
	// A class that keeps track of time in-between intervals along with the total time since the timer started recording
	class Timer
	{
	public:
		Timer();
		// Mark the time that a frame completed, recording current state
		float Mark();
		// Reset the timer as if time just started
		void Reset();
		// Get the total amount of time since the timer started recording (in seconds)
		float GetTotalTimeSeconds() const;
		// Get the total amount of time since the timer was last marked (in seconds)
		float GetDeltaTimeSeconds() const;
		// Get the total amount of time since the timer started recording (in milliseconds)
		float GetTotalTimeMilliseconds() const;
		// Get the total amount of time since the timer was last maked (in miliseconds)
		float GetDeltaTimeMilliseconds() const;
	private:
		std::chrono::steady_clock::time_point m_Last;
		float m_TotalTime;
		float m_DeltaTime;
	};
};


