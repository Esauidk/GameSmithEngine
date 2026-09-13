#include "gepch.h"
#include "Mesh.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {
	Mesh::Mesh(const std::vector<VertexData>& vertices, const std::vector<SubMesh>& subMeshes)
		: m_Verticies(vertices), m_SubMeshes(subMeshes), m_VertexBuffer(nullptr), m_IndexBuffers(m_SubMeshes.size()){}

	Ref<VertexBuffer> Mesh::GetVertexBuffer() {
	
		if (m_VertexBuffer == nullptr) {
			auto renderManager = RenderingManager::GetInstance();
			GE_CORE_ASSERT(renderManager != nullptr, "Failed to get rendering manager instance");

			m_VertexBuffer = renderManager->GetRenderAPI()->CreateVertexBuffer(
				(BYTE*)m_Verticies.data(),
				sizeof(VertexData),
				(int)m_Verticies.size()
			);
		}

		return m_VertexBuffer;
	}

	const SubMeshGPU Mesh::GetIndexBuffer(unsigned int subMeshIndex) {
		GE_CORE_ASSERT(subMeshIndex < m_SubMeshes.size(), "SubMesh index is out of bounds");

		if (m_IndexBuffers[subMeshIndex].m_Index == nullptr) {
			auto renderManager = RenderingManager::GetInstance();
			GE_CORE_ASSERT(renderManager != nullptr, "Failed to get rendering manager instance");

			SubMesh& subMesh = m_SubMeshes[subMeshIndex];
			SubMeshGPU& subMeshGPU = m_IndexBuffers[subMeshIndex];
			subMeshGPU.m_Index = renderManager->GetRenderAPI()->CreateIndexBuffer(
				subMesh.m_Indicies.data(),
				(unsigned int)subMesh.m_Indicies.size()
			);
			subMeshGPU.m_SubMeshName = m_SubMeshes[subMeshIndex].m_SubMeshName;
		}

		return m_IndexBuffers[subMeshIndex];
	}
};