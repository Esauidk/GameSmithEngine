#include "gepch.h"
#include "Shader.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Shader.h"
#endif

namespace ProjectGE {
	Shader* ProjectGE::Shader::Create(std::string path, ShaderType type)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
			break;
		case RendererAPI::API::DirectX12:
			return new DirectX12Shader(path, type);
			break;
		}

		return nullptr;
	}
};

