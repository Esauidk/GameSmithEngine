#include "gepch.h"
#include "Topology.h"

#include "ProjectGE/Rendering/RenderSettings.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12TopologyResource.h"
#endif

namespace ProjectGE {
	Topology* ProjectGE::Topology::Create(TopologyType type)
	{
		switch (RenderSettings::GetOption()) {
		case RenderOptions::NONE:
			return nullptr;
		case RenderOptions::DIRECTX12:
			return new DirectX12TopologyResource(type);
		}
		return nullptr;
	}
};

