#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/RenderAgnostics/BufferLayout.h"



namespace ProjectGE {
	// INTERFACE
	// A Buffer that holds Vertex information meant to be sent to a Render API pipeline
	class VertexBuffer
	{
		virtual void UpdateData(BYTE* data, unsigned int size) = 0;
	};
};


