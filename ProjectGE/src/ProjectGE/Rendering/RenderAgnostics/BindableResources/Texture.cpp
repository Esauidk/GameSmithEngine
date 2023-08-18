#include "gepch.h"
#include "Texture.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Texture.h"
#endif
namespace ProjectGE {
	Ref<Texture2D> ProjectGE::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::DirectX12:
			return Ref<Texture2D>(new DirectX12Texture2D(path));
		}

		return nullptr;
	}
};

