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
	void Renderer::Submit(GeometryPack* geopack, glm::mat4& objectTransform, ShaderArguement* camMat4Slot, ShaderArguement* objectMat4Slot)
	{
		camMat4Slot->SetData(&(m_SceneData->cameraMatrix));
		camMat4Slot->Bind();

		objectMat4Slot->SetData(&objectTransform);
		objectMat4Slot->Bind();

		geopack->Bind();
		RenderCommand::DrawIndexed(geopack);
	}
};
