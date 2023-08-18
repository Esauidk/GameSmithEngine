#include "gepch.h"
#include "Shader.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Shader.h"
#endif

namespace ProjectGE {
	Ref<Shader> Shader::Create(std::string vertexPath, std::string pixelPath)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
			break;
		case RendererAPI::API::DirectX12:
			return Ref<Shader>(new DirectX12Shader(vertexPath, pixelPath));
			break;
		}

		return nullptr;
	}
};

