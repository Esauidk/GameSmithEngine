#include "gepch.h"
#include "MeshAsset.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

namespace ProjectGE {
	void MeshAsset::Init()
	{
		VertexStruct* vert = (VertexStruct*)GetResourceData();
		GE_CORE_INFO("{0}, {1}, {2}", vert->pos[0], vert->pos[1], vert->pos[2]);
	}

	void MeshAsset::Destroy()
	{
	}
};