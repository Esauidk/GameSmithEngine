#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceManagement/Resource.h"

namespace ProjectGE {

	class VertexBuffer;
	class IndexBuffer;

	struct SubMesh {
		Ref<IndexBuffer> index;
	};

	class MeshAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		void SetGraphicsMesh();
	private:
		Ref<VertexBuffer> m_Vert;
		std::vector<SubMesh> m_SubMeshes;
	};
};


