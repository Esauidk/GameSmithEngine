#pragma once
#include <atomic>

namespace GameSmith {
	class Spinlock {
	public:
		Spinlock() = default;
		void Lock();
		bool IsLocked();
		void Unlock();
	private:
		std::atomic<bool> m_flag;
	};
};
