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
	void Renderer::Submit(GeometryPack* geopack, ShaderArguement* mat4Slot)
	{
		mat4Slot->SetData(&(m_SceneData->cameraMatrix));
		mat4Slot->Bind();
		geopack->Bind();
		RenderCommand::DrawIndexed(geopack);
	}
};
