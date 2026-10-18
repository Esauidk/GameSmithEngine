#pragma once

#include "MeshSerializer.h"

namespace GameSmith {
	class GE_API ObjMeshSerializer : public MeshSerializer
	{
	public:
		ObjMeshSerializer() = default;
		virtual ~ObjMeshSerializer() = default;
		virtual Ref<char> SerializeMesh(Ref<Mesh> mesh, unsigned int* outSize) override;
		virtual Ref<Mesh> DeserializeMesh(const char* data, unsigned int size) override;
	};
};