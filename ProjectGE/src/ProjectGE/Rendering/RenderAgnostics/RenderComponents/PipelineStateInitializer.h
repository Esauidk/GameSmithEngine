#pragma once
#include "Topology.h"
#include "VertexInputLayout.h"
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"

namespace ProjectGE {
	struct PipelineStateInitializer {
		bool tesselation;
		TopologyType toplopgyType;
		ShaderSet shaderSet;
	};
}
