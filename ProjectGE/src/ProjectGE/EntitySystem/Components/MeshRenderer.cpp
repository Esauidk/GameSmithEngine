#include "gepch.h"
#include "MeshRenderer.h"
#include "ProjectGE/Rendering/RenderingManager.h"

namespace ProjectGE {
	void MeshRenderer::OnUpdate()
	{
		auto renderer = RenderingManager::GetInstance();

		if (renderer != nullptr) {
			glm::mat4 model = m_Transform->GetModelMatrix();

			unsigned int size = m_Mesh->GetSubMeshSize();
			for (unsigned int i = 0; i < size; i++) {
				auto modelMatrix = m_Materials[i]->GetParameter<ShaderParameterMatrix>("Model");
				modelMatrix->SetData(model);

				renderer->Submit(m_Mesh->GetVerticies(), m_Mesh->GetSubMesh(i).m_Index, m_Materials[i]);
			}
		}
		

	}
};