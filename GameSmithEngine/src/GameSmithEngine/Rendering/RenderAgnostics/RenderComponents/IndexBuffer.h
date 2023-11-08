#pragma once

namespace GameSmith {
	// INTERFACE
	// A buffer that holds the definition of the connection between verticies
	// Ex: Vertex 0, 1, and 2 make a triangle
	class IndexBuffer
	{
	public:
		// Gets the number of indexes in the buffer
		virtual UINT GetCount() = 0;
	};
};


