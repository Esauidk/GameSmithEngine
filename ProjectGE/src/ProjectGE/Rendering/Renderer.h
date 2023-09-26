#pragma once

#include "RenderCommand.h"
#include "ProjectGE/Camera/Camera.h"

#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace ProjectGE {
	// A class that can submit high level rendering commands (Taking a scene and drawing it to the screen)
	// This can appear as handling some high level logic and then executing commands to perform low level logic related to Rendering
	class Renderer
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(Ref<GeometryPack> geopack, Ref<Shader> shader, glm::mat4& transform);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	private:
		struct SceneData {
			glm::mat4 cameraMatrix;
		};
		static SceneData* m_SceneData;
	};
};


