#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceManagement/Resource.h"

namespace ProjectGE {

	class VertexBuffer;
	class IndexBuffer;

	class MeshAsset : public Resource
	{
	public:
		MeshAsset(char* data, UINT size) : Resource(data, size) {}

		virtual void Init() override;
		virtual void Destroy() override;
	private:
		Ref<VertexBuffer> m_Vert;
		Ref<IndexBuffer> m_Index;
	};
};


