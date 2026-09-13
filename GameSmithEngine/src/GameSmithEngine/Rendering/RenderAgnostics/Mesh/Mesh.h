#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"
#include <glm/glm.hpp>

namespace GameSmith {
	struct VertexData {
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;
	};

	struct SubMesh {
		std::string m_SubMeshName;
		std::vector<unsigned int> m_Indicies;
	};

	struct SubMeshGPU {
		std::string m_SubMeshName;
		Ref<IndexBuffer> m_Index;
	};

	class Mesh {
	public:
		Mesh(const std::vector<VertexData>& vertices, const std::vector<SubMesh>& subMeshes);
		Mesh(const Mesh&) = delete;
		Mesh() = delete;
		Ref<VertexBuffer> GetVertexBuffer();
		const SubMeshGPU GetIndexBuffer(unsigned int subMeshIndex);
		unsigned int GetSubMeshSize() const { return (unsigned int)m_SubMeshes.size(); }
		const std::vector<VertexData>& GetVertices() const { return m_Verticies; }
		const std::vector<SubMesh>& GetSubMeshes() const { return m_SubMeshes; }
	private:
		std::vector<VertexData> m_Verticies;
		std::vector<SubMesh> m_SubMeshes;
		Ref<VertexBuffer> m_VertexBuffer;
		std::vector<SubMeshGPU> m_IndexBuffers;
	};
};