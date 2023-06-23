#pragma once
#include "ProjectGE/Rendering/DirectX12/PipelineState.h"

namespace ProjectGE {
	class PipelineDefiner : public BindableResource {
	public:
		// Binding the resource to the pipeline
		virtual void Setup(PipelineState& pipeline) = 0;
		virtual ~PipelineDefiner() = default;
	};
};
