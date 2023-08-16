#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"

namespace ProjectGE {
	// INTERFACE
	// Abstracts the process of defining additional arguements (not vertex information) of a shader
	class ShaderArguementDefiner : public BindableResource, public PipelineDefiner
	{
	public:
		// Requests to add an arguement
		// A wrapper to set the arguement data is returned
		virtual Ref<ShaderArguement> AddArguement(ShaderArguementType type, UINT size = 0) = 0;

		// Finializes the list of arguements to be sent to a shader 
		// (this is normally a time to serialize the arguement definition)
		virtual void FinalizeSignature() = 0;

		// Instantiates an implementation of the ShaderArguementDefiner Interface (recommended to use this instead of instantiating a specific implementation)
		static Ref<ShaderArguementDefiner> Create();
	};
};


