#pragma once
#include "gepch.h"

namespace ProjectGE {
	class Timer
	{
	public:
		Timer();
		float Mark();
		float Peek() const;
		float GetTotalTime() const;
		float GetDeltaTime() const;
	private:
		std::chrono::steady_clock::time_point m_Last;
		float m_TotalTime;
		float m_DeltaTime;
	};
};


