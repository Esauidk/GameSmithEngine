#pragma once

#include "ProjectGE/Rendering/BindableResource.h"
namespace ProjectGE {
	class IndexBuffer : public BindableResource
	{
	public:
		virtual ~IndexBuffer() {}
		static IndexBuffer* Create(void* verticies, UINT count);
	};
};


