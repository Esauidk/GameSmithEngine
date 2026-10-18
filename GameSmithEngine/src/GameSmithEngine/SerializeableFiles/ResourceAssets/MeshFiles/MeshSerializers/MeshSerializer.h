#pragma once

#include "GameSmithEngine/Rendering/RenderAgnostics/Mesh/Mesh.h"
namespace GameSmith {
	class GE_API MeshSerializer
	{
	public:
		MeshSerializer() = default;
		virtual ~MeshSerializer() = default;
		virtual Ref<char> SerializeMesh(Ref<Mesh> mesh, unsigned int* outSize) = 0;
		virtual Ref<Mesh> DeserializeMesh(const char* data, unsigned int size) = 0;
	};
};