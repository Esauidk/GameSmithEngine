#pragma once
#include "gepch.h"
#include "ProjectGE/Core/Core.h"
#include "RenderAgnostics/RenderComponents/PipelineStateInitializer.h"
#include "RenderAgnostics/RenderComponents/PipelineStateObject.h"
#include "RendererAPI.h"

namespace ProjectGE {
	class PipelineStateObjectManager {
	public:
		PipelineStateObjectManager(RendererAPI* api) : m_RenderAPI(api) {}
		Ref<GraphicsPipelineStateObject> RetrieveOrCreateStateObject(PipelineStateInitializer initializer);
		void CleanStateObjects();
	private:
		RendererAPI* m_RenderAPI;
		std::unordered_map<PipelineStateInitializer, Ref<GraphicsPipelineStateObject>, PipelineStateInitializer> m_CurrentUsed;
		std::unordered_map<PipelineStateInitializer, Ref<GraphicsPipelineStateObject>, PipelineStateInitializer> m_PreviouslyUsed;
	};
};
