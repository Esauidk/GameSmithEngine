#pragma once
#include "ProjectGE/Rendering/BindableResource.h"

namespace ProjectGE {
	class VertexBuffer : public BindableResource
	{
	public:
		virtual ~VertexBuffer() {}
		static VertexBuffer* Create(void* verticies, UINT count);
	};
};


