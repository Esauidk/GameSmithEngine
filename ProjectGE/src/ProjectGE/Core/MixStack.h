#pragma once
#include "gepch.h"

namespace ProjectGE {
	// This class is a variation of a stack data structure, along with the natural stack there is a overlay stack that will always be above any item in the vanillia stack
	template <typename T>
	class MixStack
	{
	public:
		MixStack() = default;
		~MixStack() { for (T* item : m_Items) { delete item; } };

		void Push(T* item) {
			m_Items.emplace(m_Items.begin() + m_Insert, item);
			m_Insert++;
		}

		void PushSpecial(T* item) {
			m_Items.emplace_back(item);
		}

		void Pop(T* item) {
			auto it = std::ranges::find(m_Items.begin(), m_Items.end(), item);

			if (it != m_Items.end()) {
				m_Items.erase(it);
				m_Insert--;
			}
		}

		void PopSpecial(T* item) {
			auto it = std::ranges::find(m_Items.begin(), m_Items.end(), item);

			if (it != m_Items.end()) {
				m_Items.erase(it);
			}
		}

		std::vector<T*>::iterator begin() { return m_Items.begin(); }
		std::vector<T*>::iterator end() { return m_Items.end(); }
	private:
		std::vector<T*> m_Items;
		unsigned int m_Insert = 0;
	};
};


