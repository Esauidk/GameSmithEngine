#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineStateObject.h"

namespace ProjectGE {
	// INTERFACE
	// An object that needs to modify a PipelineStateObject before it gets finialized
	// This allows the PipelineStateObject to recieve information it needs about the soon to be pipeline
	class PipelineDefiner {
	public:
		virtual ~PipelineDefiner() = default;
		// Binding the resource to the pipeline
		virtual void Append(PipelineStateObject& pipeline) = 0;
	};
};
