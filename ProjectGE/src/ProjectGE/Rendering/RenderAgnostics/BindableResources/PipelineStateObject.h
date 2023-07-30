#pragma once
#include "ProjectGE/Rendering/BindableResource.h"

namespace ProjectGE {
	class PipelineStateObject : public BindableResource {
	public:
		virtual void Build() = 0;
		static PipelineStateObject* Create();
	};
};
