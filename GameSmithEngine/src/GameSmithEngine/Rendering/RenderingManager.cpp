#include "gepch.h"
#include "RenderingManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12RendererAPI.h"
#include "GameSmithEngine/Rendering/Workflows/ForwardRender.h"

namespace GameSmith {
	RenderingManager* RenderingManager::s_Instance = nullptr;

	RenderingManager::RenderingManager()
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void RenderingManager::Init()
	{
		m_RenderAPI = Scope<RendererAPI>(new DirectX12RendererAPI());
		m_RenderWorkflow = Scope<RenderWorkflow>(new ForwardRender(m_RenderAPI.get()));
		m_PSOManager = Scope<PipelineStateObjectManager>(new PipelineStateObjectManager(m_RenderAPI.get()));
		m_SceneDataGPU = m_RenderAPI->CreateConstantBuffer(sizeof(GloablShaderData));
		GE_CORE_INFO("Rendering Manager Loaded!");
	}

	void RenderingManager::SetRenderWorkflow(RenderWorkflow* workflow)
	{
		m_RenderWorkflow = Scope<RenderWorkflow>(workflow);
	}

	void RenderingManager::ShutDown()
	{
		m_SceneDataGPU = nullptr;
		m_RenderAPI = nullptr;
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
			m_SceneData.MainLightColor = glm::vec3(1, 1, 1);
		}
		
		m_SceneDataGPU->UpdateData((BYTE*)&m_SceneData, sizeof(m_SceneData));
		m_RenderAPI->SetConstantBuffer(m_SceneDataGPU, STAGE_VERTEX, ShaderConstantType::Global);
		m_RenderAPI->SetConstantBuffer(m_SceneDataGPU, STAGE_PIXEL, ShaderConstantType::Global);
	}

	void RenderingManager::EndScene()
	{
		
	}

	void RenderingManager::EndFrame()
	{
		EndScene();
		m_RenderAPI->CompleteFrameSubmissions();
		m_PSOManager->CleanStateObjects();
	}

	void RenderingManager::Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat)
	{
		m_RenderWorkflow->Submit(vBuff, iBuff, mat);
		m_RenderAPI->SubmitRecording();
	}

	
};
