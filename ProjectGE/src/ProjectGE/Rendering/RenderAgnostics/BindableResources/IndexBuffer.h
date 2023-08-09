#pragma once

#include "ProjectGE/Rendering/BindableResource.h"
namespace ProjectGE {
	// INTERFACE
	// A buffer that holds the definition of the connection between verticies
	// Ex: Vertex 0, 1, and 2 make a triangle
	class IndexBuffer : public BindableResource
	{
	public:
		virtual ~IndexBuffer() {}
		// Instantiates an implementation of the IndexBuffer Interface (recommended to use this instead of instantiating a specific implementation)
		static IndexBuffer* Create(void* verticies, UINT count);
		// Gets the number of indexes in the buffer
		virtual UINT GetCount() = 0;
	};
};


