#include "gepch.h"
#include "Renderer.h"


namespace ProjectGE {
	Renderer::SceneData* Renderer::m_SceneData = new SceneData();
	void Renderer::BeginScene(Camera& cam)
	{
		m_SceneData->cameraMatrix = cam.GetMatrix();
	}

	void Renderer::EndScene()
	{
		RenderCommand::FinishedRecording();
	}
	void Renderer::Submit(std::shared_ptr<GeometryPack> geopack, std::shared_ptr<Shader> shader, glm::mat4& objectTransform, std::shared_ptr<ShaderArguement> camMat4Slot, std::shared_ptr<ShaderArguement> objectMat4Slot)
	{
		shader->Bind();

		camMat4Slot->SetData(&(m_SceneData->cameraMatrix));
		camMat4Slot->Bind();

		objectMat4Slot->SetData(&objectTransform);
		objectMat4Slot->Bind();

		geopack->Bind();
		RenderCommand::DrawIndexed(geopack);
	}
};
