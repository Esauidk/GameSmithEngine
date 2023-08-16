#include "gepch.h"
#include "Topology.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12TopologyResource.h"
#endif

namespace ProjectGE {
	Ref<Topology> ProjectGE::Topology::Create(TopologyType type)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::DirectX12:
			return Ref<Topology>(new DirectX12TopologyResource(type));
		}
		return nullptr;
	}
};

