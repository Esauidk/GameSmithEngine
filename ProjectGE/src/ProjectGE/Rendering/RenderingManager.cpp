#include "gepch.h"
#include "RenderingManager.h"
#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12RendererAPI.h"

namespace ProjectGE {
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
		m_SceneDataGPU = m_RenderAPI->CreateConstantBuffer(sizeof(GloablShaderData));
		GE_CORE_INFO("Rendering Manager Loaded!");
	}

	void RenderingManager::ShutDown()
	{
		m_SceneDataGPU = nullptr;
		m_RenderAPI = nullptr;
	}

	void RenderingManager::BeginScene(Camera* cam, LightSource* mainLight)
	{
		m_SceneData.VP = cam->GetMatrix();

		if (mainLight != nullptr) {
			m_SceneData.MainLightDir = mainLight->GetLightVector();
			m_SceneData.MainLightColor = mainLight->GetLightColor();
		}
		else {
			m_SceneData.MainLightDir = glm::vec3(0, 0, 0);
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

	void RenderingManager::Submit(Ref<GeometryPack> geopack, Ref<Shader> shader, glm::mat4& objectTransform)
	{
		//shader->Bind();

		//geopack->Bind();
		//RenderCommand::DrawIndexed(geopack);
	}
};
