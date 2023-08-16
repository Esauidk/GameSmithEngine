#include "gepch.h"
#include "GeometryPack.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12GeometryPack.h"
#endif

namespace ProjectGE {
	Ref<GeometryPack> GeometryPack::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::DirectX12:
			return Ref<GeometryPack>(new DirectX12GeometryPack());
		}

		return nullptr;
	}
};