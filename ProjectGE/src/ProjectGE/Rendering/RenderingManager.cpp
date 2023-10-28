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
		m_SceneData.CameraWorldPos = cam->GetTransform().GetPosition();
		// TODO: Remove this
		m_SceneData.LightWorldPos = glm::vec3(0, 0, 0);

		if (mainLight != nullptr) {
			m_SceneData.MainLightDir = mainLight->GetLightVector();
			m_SceneData.MainLightColor = mainLight->GetLightColor();
		}
		else {
			m_SceneData.MainLightDir = glm::vec3(1, 0, 1);
			m_SceneData.MainLightColor = glm::vec3(1, 1, 1);
		}
		sizeof(glm::mat4);
		sizeof(glm::vec3);
		GE_CORE_INFO("Light Position: {0}, {1}, {2}", m_SceneData.LightWorldPos.x, m_SceneData.LightWorldPos.y, m_SceneData.LightWorldPos.z);
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
