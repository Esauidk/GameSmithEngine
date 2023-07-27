#pragma once
#include "ProjectGE/Rendering/BindableResource.h"

namespace ProjectGE {
	class PipelineStateObject : public BindableResource {
		virtual void Build() = 0;
	};
};
