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
		GE_CORE_INFO("Rendering Manager Loaded!");
	}

	void RenderingManager::ShutDown()
	{
	}

	void RenderingManager::BeginScene(Camera& cam)
	{
		//m_SceneData->cameraMatrix = cam.GetMatrix();
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
