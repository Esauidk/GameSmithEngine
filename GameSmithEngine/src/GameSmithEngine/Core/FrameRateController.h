#pragma once
namespace GameSmith {
	class FrameRateController
	{
	public:
		FrameRateController();
		inline void SetFrameRate(float frameRate) { m_FrameRate = frameRate; }
		inline void SetFrameRateLimiting(bool state) { m_LimitExists = state; }
		void CheckAndPerformDelay();
	private:
		float m_FrameRate;
		bool m_LimitExists;
	};
};


