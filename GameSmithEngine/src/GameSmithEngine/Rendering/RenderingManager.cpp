#include "gepch.h"
#include "RenderingManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12RendererAPI.h"
#include "GameSmithEngine/Rendering/Workflows/ForwardRender.h"

namespace GameSmith {
	RenderingManager* RenderingManager::s_Instance = nullptr;

	RenderingManager::RenderingManager()
	{
		m_RenderAPI = Scope<RendererAPI>(new DirectX12RendererAPI());
		m_RenderWorkflow = Scope<RenderWorkflow>(new ForwardRender(m_RenderAPI.get()));
		m_PSOManager = Scope<PipelineStateObjectManager>(new PipelineStateObjectManager(m_RenderAPI.get()));
		m_SceneDataGPU = m_RenderAPI->CreateConstantBuffer(sizeof(GloablShaderData));
	}

	void RenderingManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new RenderingManager();
			GE_CORE_INFO("Rendering Manager Loaded!");
		}
	}

	void RenderingManager::SetRenderWorkflow(RenderWorkflow* workflow)
	{
		m_RenderWorkflow = Scope<RenderWorkflow>(workflow);
	}

	void RenderingManager::ShutDown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	void RenderingManager::BeginScene(Camera* cam, LightSource* mainLight)
	{
		//m_RenderAPI->ClearCachedAssets();
		m_SceneData.VP = cam->GetMatrix();
		m_SceneData.CameraWorldPos = cam->GetTransform().GetPosition();

		if (mainLight != nullptr) {
			m_SceneData.LightWorldPos = mainLight->GetLightVector();
			m_SceneData.MainLightColor = mainLight->GetLightColor();
		}
		else {
			m_SceneData.LightWorldPos = glm::vec4(1, 0, 1, 0);
			m_SceneData.MainLightColor = glm::vec3(0, 0, 0);
		}
		
		m_SceneDataGPU->UpdateData((BYTE*)&m_SceneData, sizeof(m_SceneData));
		m_RenderAPI->SetConstantBuffer(m_SceneDataGPU, STAGE_VERTEX, ShaderConstantType::Global);
		m_RenderAPI->SetConstantBuffer(m_SceneDataGPU, STAGE_PIXEL, ShaderConstantType::Global);
	}

	void RenderingManager::EndScene()
	{
		m_RenderAPI->SubmitRecording();
	}

	void RenderingManager::EndFrame()
	{
		EndScene();
		m_RenderAPI->CompleteFrameSubmissions();
		m_PSOManager->CleanStateObjects();

		EndFrameEvent e;
		m_FrameEndDispatch.Dispatch(e);
	}

	void RenderingManager::ClearTextures()
	{
		while (!m_ClearOnSwap.empty()) {
			auto tex = m_ClearOnSwap.front();
			m_ClearOnSwap.pop();

			tex->ClearTexture();
		}
	}

	void RenderingManager::Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat)
	{
		m_RenderWorkflow->Submit(vBuff, iBuff, mat);
		m_RenderAPI->SubmitRecording();
	}

	
};
