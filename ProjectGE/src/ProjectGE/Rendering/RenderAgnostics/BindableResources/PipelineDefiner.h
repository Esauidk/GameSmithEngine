#pragma once
#include "PipelineStateObject.h"

namespace ProjectGE {
	class PipelineDefiner {
	public:
		virtual ~PipelineDefiner() {}
		// Binding the resource to the pipeline
		virtual void Append(PipelineStateObject& pipeline) = 0;
	};
};
