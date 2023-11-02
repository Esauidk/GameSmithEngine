#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceManagement/Resource.h"

namespace ProjectGE {

	class VertexBuffer;
	class IndexBuffer;

	struct SubMesh {
		std::string m_SubMeshName;
		Ref<IndexBuffer> m_Index;
	};

	class MeshAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		inline Ref<VertexBuffer> GetVerticies() { return m_Vert; }
		inline SubMesh GetSubMesh(unsigned int index) { return m_SubMeshes[index]; }
		inline unsigned int GetSubMeshSize() { return (unsigned int)m_SubMeshes.size(); }
	private:
		Ref<VertexBuffer> m_Vert;
		std::vector<SubMesh> m_SubMeshes;
	};
};


