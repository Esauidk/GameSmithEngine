#pragma once
#include "BindableResource.h"
#include "PipelineState.h"

namespace Render {
	class PipelineDefiner : public BindableResource {
	public:
		// Binding the resource to the pipeline
		virtual void Setup(PipelineState& pipeline) = 0;
		virtual ~PipelineDefiner() = default;
	};
};
