#pragma once
#include "MeshAsset.h"
#include "MeshSerializers/ObjMeshSerializer.h"

namespace GameSmith {
	class GE_API ObjMeshAsset : public MeshAsset
	{
	public:
		ObjMeshAsset(std::string name) : MeshAsset(name, Scope<MeshSerializer>(new ObjMeshSerializer())) {}
		ObjMeshAsset(std::string name, Ref<Mesh> mesh) : MeshAsset(name, mesh, Scope<MeshSerializer>(new ObjMeshSerializer())) {}
		SERIAL_FILE(OBJ, obj)
	};
};