#include "gepch.h"
#include "PipelineStateObjectManager.h"

namespace GameSmith {
	Ref<GraphicsPipelineStateObject> PipelineStateObjectManager::RetrieveOrCreateStateObject(PipelineStateInitializer initializer)
	{
		if (m_CurrentMap->contains(initializer)) {
			return m_CurrentMap->find(initializer)->second;
		}
		else if (m_CleanupMap->contains(initializer)) {
			auto pso = m_CleanupMap->find(initializer)->second;
			m_CurrentMap->insert({ initializer, pso });

			return pso;
		}

		auto newPso = m_RenderAPI->CreateGraphicsPipelineState(initializer);
		m_CurrentMap->insert({ initializer, newPso });

		return newPso;
	}

	void PipelineStateObjectManager::CleanStateObjects()
	{
		for (auto it = m_CleanupMap->begin(); it != m_CleanupMap->end();) {
			if (it->second.use_count() == 1) {
				m_CleanupMap->erase(it++);
			}
			else {
				++it;
			}
		}

		if (m_CurrentMap == &m_Map1) {
			m_CurrentMap = &m_Map2;
			m_CleanupMap = &m_Map1;
		}
		else {
			m_CurrentMap = &m_Map1;
			m_CleanupMap = &m_Map2;
		}
	}
};