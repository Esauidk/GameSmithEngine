#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Mesh/Mesh.h"

namespace GameSmith {
	Ref<Mesh> GE_API CreateCube(unsigned int size);
	Ref<Mesh> GE_API CreateSphere();
	Ref<Mesh> GE_API CreateCylinder();
	Ref<Mesh> GE_API CreateCone();
	Ref<Mesh> GE_API CreateTorus();
	Ref<Mesh> GE_API CreatePlane();
	Ref<Mesh> GE_API CreateCapsule();
};