#include "gepch.h"
#include "PipelineStateObjectManager.h"

namespace ProjectGE {
	Ref<GraphicsPipelineStateObject> PipelineStateObjectManager::RetrieveOrCreateStateObject(PipelineStateInitializer initializer)
	{
		if (m_CurrentUsed.contains(initializer)) {
			return m_CurrentUsed.find(initializer)->second;
		}
		else if (m_PreviouslyUsed.contains(initializer)) {
			auto pso = m_PreviouslyUsed.find(initializer)->second;
			m_CurrentUsed.insert({ initializer, pso });

			return pso;
		}

		auto newPso = m_RenderAPI->CreateGraphicsPipelineState(initializer);
		m_CurrentUsed.insert({ initializer, newPso });

		return newPso;
	}

	void PipelineStateObjectManager::CleanStateObjects()
	{
	}
};