#pragma once
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"

namespace ProjectGE {
	class ShaderArguementDefiner : public BindableResource, public PipelineDefiner
	{
	public:
		virtual ShaderArguement* AddArguement(void* data, UINT size, ShaderArguementType type) = 0;
		virtual void FinalizeSignature() = 0;
		static ShaderArguementDefiner* Create();
	};
};


