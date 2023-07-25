#pragma once
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"

namespace ProjectGE {
	class DirectX12PipelineDefiner : public BindableResource {
	public:
		// Binding the resource to the pipeline
		virtual void Setup(DirectX12PipelineState& pipeline) = 0;
	};
};
