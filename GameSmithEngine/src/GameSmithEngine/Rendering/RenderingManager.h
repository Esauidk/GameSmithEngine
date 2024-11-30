#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "RendererAPI.h"
#include "PipelineStateObjectManager.h"
#include "RenderWorkflow.h"
#include "GameSmithEngine/Events/RenderingEvents.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/Camera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/LightSource.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"

namespace GameSmith {
	// A class that can submit high level rendering commands (Taking a scene and drawing it to the screen)
	// This can appear as handling some high level logic and then executing commands to perform low level logic related to Rendering
	class GE_API RenderingManager
	{
	public:
		inline static RenderingManager* GetInstance() { return s_Instance; }

		static void Init();
		static void ShutDown();
		void SetRenderWorkflow(RenderWorkflow* workflow);
		void BeginScene(Camera* cam, LightSource* mainLight);
		void EndScene();
		void EndFrame();
		inline void SetFrameTexture(Ref<RenderTexture> frameTexture) { m_FrameTexture = frameTexture; }
		inline void SetForClear(Ref<RenderTexture> m_Tex) { m_ClearOnSwap.emplace(m_Tex); }

		void ClearTextures();
		void Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat);

		inline RendererAPI::API GetAPI() { return m_RenderAPI->GetAPI(); };
		inline RendererAPI* GetRenderAPI() { return m_RenderAPI.get(); }
		inline PipelineStateObjectManager* GetPSOManager() { return m_PSOManager.get(); }
		inline Ref<RenderTexture> GetTextureForFrame() { return m_FrameTexture; }

		// Returns a list of event dispatchers, used when you want to subscribe to a Window event
		inline const std::vector<EventDispatcherBase*>& GetDistpachers() const { return m_Dispatchers; }
	private:
		RenderingManager();
	private:
		static RenderingManager* s_Instance;
		Scope<RendererAPI> m_RenderAPI;
		Scope<PipelineStateObjectManager> m_PSOManager;
		Scope<RenderWorkflow> m_RenderWorkflow;
		GloablShaderData m_SceneData;
		Ref<ConstantBuffer> m_SceneDataGPU;
		Ref<RenderTexture> m_FrameTexture;
		std::queue<Ref<RenderTexture>> m_ClearOnSwap;

		EventDispatcher<EndFrameEvent> m_FrameEndDispatch;

		const std::vector<EventDispatcherBase*> m_Dispatchers = { &m_FrameEndDispatch};
	};
};


