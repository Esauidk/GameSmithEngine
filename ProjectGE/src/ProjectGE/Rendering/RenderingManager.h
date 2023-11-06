#pragma once
#include "RendererAPI.h"
#include "PipelineStateObjectManager.h"

#include "ProjectGE/Rendering/RenderAgnostics/Camera/Camera.h"
#include "ProjectGE/Rendering/RenderAgnostics/LightingSystem/LightSource.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/MaterialSystem/Material.h"

namespace ProjectGE {
	// A class that can submit high level rendering commands (Taking a scene and drawing it to the screen)
	// This can appear as handling some high level logic and then executing commands to perform low level logic related to Rendering
	class RenderingManager
	{
	public:
		RenderingManager();
		inline static RenderingManager* GetInstance() { return s_Instance; }

		void Init();
		void ShutDown();
		void BeginScene(Camera* cam, LightSource* mainLight);
		void EndScene();
		void Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat);

		inline RendererAPI::API GetAPI() { return m_RenderAPI->GetAPI(); };
		inline RendererAPI* GetRenderAPI() { return m_RenderAPI.get(); }
		inline PipelineStateObjectManager* GetPSOManager() { return m_PSOManager.get(); }
	private:
		static RenderingManager* s_Instance;
		Scope<RendererAPI> m_RenderAPI;
		Scope<PipelineStateObjectManager> m_PSOManager;
		GloablShaderData m_SceneData;
		Ref<ConstantBuffer> m_SceneDataGPU;
	};
};


