#pragma once
#include "ProjectGE/Core.h"
#include "gepch.h"

namespace ProjectGE {
	template <typename T>
	class MixStack
	{
	public:
		MixStack() { m_Insert = m_Items.begin(); };
		~MixStack() { for (T* item : m_Items) { delete item; } };

		void Push(T* item) {
			m_Insert = m_Items.emplace(m_Insert, item);
		}

		void PushSpecial(T* item) {
			m_Items.emplace_back(item);
		}

		void Pop(T* item) {
			auto it = std::find(m_Items.begin(), m_Items.end(), item);

			if (it != m_Items.end()) {
				m_Items.erase(it);
				m_Insert--;
			}
		}

		void PopSpecial(T* item) {
			auto it = std::find(m_Items.begin(), m_Items.end(), item);

			if (it != m_Items.end()) {
				m_Items.erase(it);
				m_Insert--;
			}
		}

		std::vector<T*>::iterator begin() { return m_Items.begin(); }
		std::vector<T*>::iterator end() { return m_Items.end(); }
	private:
		std::vector<T*> m_Items;
		std::vector<T*>::iterator m_Insert;
	};
};


