#pragma once
#include "Locks/Spinlock.h"
#include <queue>

namespace GameSmith {
	template<typename T>
	class ThreadSafeQueue {
	public:
		void Push(T item) {
			m_Lock.Lock();
			m_Queue.push(item);
			m_Lock.Unlock();
		}

		T Pop() {
			m_Lock.Lock();
			const T item = m_Queue.front();
			m_Queue.pop();
			m_Lock.Unlock();

			return item;
		}

		bool IsEmpty() {
			m_Lock.Lock();
			const bool empty = m_Queue.empty();
			m_Lock.Unlock();
			return empty;
		}
	private:
		Spinlock m_Lock;
		std::queue<T> m_Queue;
	};
};
