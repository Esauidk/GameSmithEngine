#pragma once
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"

namespace ProjectGE{
	enum class TopologyType {
		None = 0,
		Triangle,
		Square,
		Point
	};

	class Topology : public PipelineDefiner, public BindableResource
	{
	public:
		static Topology* Create(TopologyType type);
	};
};


