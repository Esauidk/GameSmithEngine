#pragma once
#include "Topology.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"

namespace GameSmith {
	struct PipelineStateInitializer {
		bool tesselation = false;
		TopologyType toplopgyType = TopologyType::Triangle;
		ShaderSet shaderSet;

		inline std::size_t operator()(const PipelineStateInitializer& key) const {
			std::size_t hash = 0;

			hash |= (std::size_t)tesselation | (std::size_t)toplopgyType | shaderSet(shaderSet);

			return hash;
		}

		inline bool operator==(const PipelineStateInitializer& rhs) const {
			return !(tesselation != rhs.tesselation ||
				toplopgyType != rhs.toplopgyType ||
				shaderSet.shaders[STAGE_VERTEX] != rhs.shaderSet.shaders[STAGE_VERTEX] ||
				shaderSet.shaders[STAGE_HULL] != rhs.shaderSet.shaders[STAGE_HULL] ||
				shaderSet.shaders[STAGE_DOMAIN] != rhs.shaderSet.shaders[STAGE_DOMAIN] ||
				shaderSet.shaders[STAGE_PIXEL] != rhs.shaderSet.shaders[STAGE_PIXEL]);
		}
	};
}
