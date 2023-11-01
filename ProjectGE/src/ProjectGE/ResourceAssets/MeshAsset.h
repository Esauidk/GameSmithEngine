#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceManagement/Resource.h"

namespace ProjectGE {

	class VertexBuffer;
	class IndexBuffer;

	class SubMesh {
	public:
		SubMesh(std::string name, Ref<IndexBuffer> index);
		void DrawSubMesh();
		inline std::string GetName() { return m_SubMeshName; }
	private:
		std::string m_SubMeshName;
		Ref<IndexBuffer> m_Index;
	};

	class MeshAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		void SetGraphicsMesh();
		inline void DrawMesh(unsigned int index) { m_SubMeshes[index].DrawSubMesh(); }
		inline unsigned int GetSubMeshSize() { return (unsigned int)m_SubMeshes.size(); }
	private:
		Ref<VertexBuffer> m_Vert;
		std::vector<SubMesh> m_SubMeshes;
	};
};


