#pragma once
#include "gepch.h"
#include "Locks/Spinlock.h"

namespace GameSmith {
	template<typename T>
	class ThreadSafeVector {
	public:
		ThreadSafeVector() = default;
		~ThreadSafeVector() = default;

		void PushBack(const T& item) {
			m_lock.Lock();
			m_inner.push_back(item);
			m_lock.Unlock();
		}

		size_t Size() {
			m_lock.Lock();
			size_t size = m_inner.size();
			m_lock.Unlock();
			return size;
		}

		T Get(size_t index) {
			m_lock.Lock();
			T& item = m_inner.at(index);
			m_lock.Unlock();
			return item;
		}

		void Remove(size_t index) {
			m_lock.Lock();
			m_inner.erase(m_inner.begin() + index);
			m_lock.Unlock();
		}

		bool isEmpty() {
			m_lock.Lock();
			bool empty = m_inner.empty();
			m_lock.Unlock();
			return empty;
		}

	private:
		std::vector<T> m_inner;
		Spinlock m_lock;
	};
};

