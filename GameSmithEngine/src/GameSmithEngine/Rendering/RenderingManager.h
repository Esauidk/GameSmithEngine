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

		/// <summary>
		/// Intializes the inner state of the manager, must be called before making render calls
		/// </summary>
		static void Init();

		/// <summary>
		/// Tearsdown the manager's inner state, render calls will no longer work until Init() is called again
		/// </summary>
		static void ShutDown();

		/// <summary>
		/// Sets the render workflow to be used when processing a submission
		/// 
		/// Will take over ownership over workflow instance
		/// </summary>
		/// <param name="workflow"> A pointer to the workflow to use </param>
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
		// Intentionally disable constructor for clients
		RenderingManager();
	private:
		// Global reference to self
		static RenderingManager* s_Instance;

		// Render API Abstraction
		Scope<RendererAPI> m_RenderAPI;

		// Internal manager for PSOs 
		Scope<PipelineStateObjectManager> m_PSOManager;

		// The guideline workflow for how to handle a render submission
		Scope<RenderWorkflow> m_RenderWorkflow;

		// Shared data to use for all submissions
		GloablShaderData m_SceneData;

		// GPU memory for shared data
		Ref<ConstantBuffer> m_SceneDataGPU;

		// The texture to render submissions to
		Ref<RenderTexture> m_FrameTexture;

		// A list of textures to clear after a swap chain switch
		std::queue<Ref<RenderTexture>> m_ClearOnSwap;

		// A dispatcher to notify that a frame has ended
		EventDispatcher<EndFrameEvent> m_FrameEndDispatch;

		// List of dispatchers available
		const std::vector<EventDispatcherBase*> m_Dispatchers = { &m_FrameEndDispatch};
	};
};


