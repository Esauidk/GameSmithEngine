#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Mesh/Mesh.h"

namespace GameSmith {
	Ref<Mesh> CreateCube(unsigned int size);
	Ref<Mesh> CreateSphere();
	Ref<Mesh> CreateCylinder();
	Ref<Mesh> CreateCone();
	Ref<Mesh> CreateTorus();
	Ref<Mesh> CreatePlane();
	Ref<Mesh> CreateCapsule();
};