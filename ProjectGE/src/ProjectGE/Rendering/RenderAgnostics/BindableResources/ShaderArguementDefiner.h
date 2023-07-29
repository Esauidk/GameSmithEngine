#pragma once
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"

namespace ProjectGE {
	class ShaderArguementDefiner : public BindableResource
	{
		virtual int AddArguement(ShaderArguement& arg) = 0;
		static ShaderArguementDefiner* Create();
	};
};


