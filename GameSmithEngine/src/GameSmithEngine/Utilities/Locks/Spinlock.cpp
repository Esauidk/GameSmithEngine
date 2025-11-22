#include "gepch.h"
#include "Spinlock.h"

namespace GameSmith {

	// https://rigtorp.se/spinlock/

	void Spinlock::Lock() {
		for (;;) {
			if (!m_flag.exchange(true, std::memory_order_acquire)) {
				break;
			}

			while (m_flag.load(std::memory_order_relaxed));
		}
		
	}

	bool Spinlock::IsLocked()
	{
		return m_flag.load(std::memory_order_relaxed);
	}

	void Spinlock::Unlock() {
		m_flag.store(false, std::memory_order_release);
	}
};