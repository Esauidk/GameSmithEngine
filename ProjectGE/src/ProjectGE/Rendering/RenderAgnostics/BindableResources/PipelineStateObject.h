#pragma once
#include "ProjectGE/Rendering/BindableResource.h"

namespace ProjectGE {
	// INTERFACE
	// An object that defines the entire pipeline order and properites to be used on the next draw
	class PipelineStateObject : public BindableResource {
	public:
		// Finializes the object
		// This is a chance to serialize the object if need be
		virtual void Build() = 0;
		// Instantiates an implementation of the PipelineStateObject Interface (recommended to use this instead of instantiating a specific implementation)
		static PipelineStateObject* Create();
	};
};
