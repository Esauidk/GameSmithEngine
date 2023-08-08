#pragma once

#include "RenderCommand.h"
#include "ProjectGE/Camera/Camera.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"

namespace ProjectGE {
	
	class Renderer
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(GeometryPack* geopack, ShaderArguement* mat4Slot);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	private:
		struct SceneData {
			glm::mat4 cameraMatrix;
		};
		static SceneData* m_SceneData;
	};
};


