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
		RenderCommand::SubmitRecording();
	}
	void Renderer::Submit(Ref<GeometryPack> geopack, Ref<Shader> shader, glm::mat4& objectTransform)
	{
		shader->Bind();

		geopack->Bind();
		//RenderCommand::DrawIndexed(geopack);
	}
};
