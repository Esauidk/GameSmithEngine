#pragma once
#include "Asset.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {

	class VertexBuffer;
	class IndexBuffer;

	struct SubMesh {
		std::string m_SubMeshName;
		Ref<IndexBuffer> m_Index;
	};

	class GE_API MeshAsset : public Asset
	{
	public:
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		inline Ref<VertexBuffer> GetVerticies() { return m_Vert; }
		inline SubMesh GetSubMesh(unsigned int index) { return m_SubMeshes[index]; }
		inline unsigned int GetSubMeshSize() { return (unsigned int)m_SubMeshes.size(); }

		SERIAL_FILE(Mesh, mesh)
	private:
		Ref<VertexBuffer> m_Vert;
		std::vector<SubMesh> m_SubMeshes;
	};
};


