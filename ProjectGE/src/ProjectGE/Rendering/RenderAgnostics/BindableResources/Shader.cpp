#include "gepch.h"
#include "Shader.h"

#include "ProjectGE/Rendering/RenderSettings.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Shader.h"
#endif

namespace ProjectGE {
	Shader* ProjectGE::Shader::Create(std::string path, ShaderType type)
	{
		switch (RenderSettings::GetOption()) {
		case RenderOptions::NONE:
			return nullptr;
			break;
		case RenderOptions::DIRECTX12:
			return new DirectX12Shader(path, type);
			break;
		}

		return nullptr;
	}
};

