#include "gepch.h"
#include "GameplayUpdater.h"

namespace GameSmith {
	GameplayUpdater* GameplayUpdater::s_Instance = nullptr;

	GameplayUpdater::GameplayUpdater()
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void GameplayUpdater::RunGameplayInit()
	{
		while (!m_InitQueue.empty()) {
			auto comp = m_InitQueue.top();

			if (!comp.expired()) {
				comp.lock()->Init();
			}

			m_InitQueue.pop();
		}
	}

	void GameplayUpdater::RunGameplayUpdate()
	{
		if (std::is_heap(m_RegisteredComponents.begin(), m_RegisteredComponents.end(), ComponentCompare())) {
			std::make_heap(m_RegisteredComponents.begin(), m_RegisteredComponents.end(), ComponentCompare());
		}
		
		for (auto it = m_RegisteredComponents.begin(); it != m_RegisteredComponents.end();) {
			if (it->expired()) {
				it = m_RegisteredComponents.erase(it);
			}
			else {
				it->lock()->OnUpdate();
				it++;
			}
		}
	}
};
