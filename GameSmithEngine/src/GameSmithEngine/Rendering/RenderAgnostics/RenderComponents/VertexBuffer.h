#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/BufferLayout.h"



namespace GameSmith {
	// INTERFACE
	// A Buffer that holds Vertex information meant to be sent to a Render API pipeline
	class VertexBuffer
	{
		virtual void UpdateData(BYTE* data, unsigned int size) = 0;
	};
};


