#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"

namespace ProjectGE{
	enum class TopologyType {
		None = 0,
		Triangle,
		Square,
		Point
	};

	// INTERFACE
	// A encasulation of the type of topology meant to be used when rendering vertcies
	class Topology : public PipelineDefiner, public BindableResource
	{
	public:
		// Instantiates an implementation of the Topology Interface (recommended to use this instead of instantiating a specific implementation)
		static Ref<Topology> Create(TopologyType type);
	};
};


