#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineStateObject.h"

namespace ProjectGE {
	class PipelineDefiner {
	public:
		virtual ~PipelineDefiner() {}
		// Binding the resource to the pipeline
		virtual void Append(PipelineStateObject& pipeline) = 0;
	};
};
