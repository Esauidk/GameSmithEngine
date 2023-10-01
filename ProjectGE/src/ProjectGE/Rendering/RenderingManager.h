#pragma once
#include "RendererAPI.h"

#include "ProjectGE/Camera/Camera.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace ProjectGE {
	// A class that can submit high level rendering commands (Taking a scene and drawing it to the screen)
	// This can appear as handling some high level logic and then executing commands to perform low level logic related to Rendering
	class RenderingManager
	{
	public:
		RenderingManager();
		inline static RenderingManager* GetInstance() { return s_Instance; }

		void Init();
		void ShutDown();
		void BeginScene(Camera& camera);
		void EndScene();

		void Submit(Ref<GeometryPack> geopack, Ref<Shader> shader, glm::mat4& transform);
		inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
		inline RendererAPI* GetRenderAPI() { return m_RenderAPI.get(); }
	private:
		static RenderingManager* s_Instance;
		Scope<RendererAPI> m_RenderAPI;
	};
};


