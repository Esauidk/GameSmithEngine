#pragma once
#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	// INTERFACE
	// A buffer that holds the definition of the connection between verticies
	// Ex: Vertex 0, 1, and 2 make a triangle
	class IndexBuffer
	{
	public:
		// Instantiates an implementation of the IndexBuffer Interface (recommended to use this instead of instantiating a specific implementation)
		static Ref<IndexBuffer> Create(void* verticies, UINT count);
		// Gets the number of indexes in the buffer
		virtual UINT GetCount() = 0;
	};
};


