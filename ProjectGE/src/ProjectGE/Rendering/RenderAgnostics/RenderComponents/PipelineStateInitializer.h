#pragma once
#include "Topology.h"
#include "Shader.h"
#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	struct PipelineStateInitializer {
		void* vertexLayout;
		TopologyType toplopgyType;
		Ref<Shader> shaders[ShaderCount];
	};
}
