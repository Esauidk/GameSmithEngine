#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "RenderAgnostics/RenderComponents/PipelineStateInitializer.h"
#include "RenderAgnostics/RenderComponents/PipelineStateObject.h"
#include "RendererAPI.h"

namespace GameSmith {
	class PipelineStateObjectManager {
	public:
		PipelineStateObjectManager(RendererAPI* api) : m_RenderAPI(api), m_CurrentMap(&m_Map1), m_CleanupMap(&m_Map2) {}
		Ref<GraphicsPipelineStateObject> RetrieveOrCreateStateObject(PipelineStateInitializer initializer);
		void CleanStateObjects();
	private:
		RendererAPI* m_RenderAPI;
		std::unordered_map<PipelineStateInitializer, Ref<GraphicsPipelineStateObject>, PipelineStateInitializer> m_Map1;
		std::unordered_map<PipelineStateInitializer, Ref<GraphicsPipelineStateObject>, PipelineStateInitializer> m_Map2;
		std::unordered_map<PipelineStateInitializer, Ref<GraphicsPipelineStateObject>, PipelineStateInitializer>* m_CurrentMap;
		std::unordered_map<PipelineStateInitializer, Ref<GraphicsPipelineStateObject>, PipelineStateInitializer>* m_CleanupMap;
	};
};
