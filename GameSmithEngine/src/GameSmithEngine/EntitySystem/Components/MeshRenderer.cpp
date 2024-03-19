#include "gepch.h"
#include "MeshRenderer.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/EntitySystem/GameObject.h"

namespace GameSmith {
	void MeshRenderer::OnUpdate(float dt)
	{
		auto renderer = RenderingManager::GetInstance();

		if (renderer != nullptr && m_Mesh != nullptr) {
			glm::mat4 model = m_Transform->GetModelMatrix();
			bool changed = m_Transform->HasChanged();
			unsigned int size = m_Mesh->GetSubMeshSize();
			for (unsigned int i = 0; i < size; i++) {
				if (changed) {
					auto modelMatrix = m_Materials[i]->GetParameter<MatrixContainor>("Model");
					modelMatrix->SetData(model);
				}
				
				renderer->Submit(m_Mesh->GetVerticies(), m_Mesh->GetSubMesh(i).m_Index, m_Materials[i]);
			}
		}
		else {
			GE_CORE_INFO("Something went wrong updating");
		}
		

	}
};