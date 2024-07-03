#include "gepch.h"
#include "MeshRenderer.h"
#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/EntitySystem/Components/ComponentFactory.h"

namespace GameSmith {
	GE_REGISTERCOMPONENT(MeshRenderer)

	void MeshRenderer::OnUpdate(float dt)
	{
		auto renderPrep = EntityRenderPreparer::GetInstance();

		if (renderPrep != nullptr && m_Mesh != nullptr) {
			glm::mat4 model = m_Transform->GetModelMatrix();
			bool changed = m_Transform->HasChanged();
			unsigned int size = m_Mesh->GetSubMeshSize();
			for (unsigned int i = 0; i < size; i++) {
				if (m_Materials[i].first == nullptr) {
					continue;
				}

				if (changed) {
					auto modelMatrix = m_Materials[i].first->GetParameter<MatrixContainor>("Model");
					modelMatrix->SetData(model);
				}
				
				renderPrep->AddRenderRequest({m_Mesh->GetVerticies(), m_Mesh->GetSubMesh(i).m_Index, m_Materials[i].first});
			}
		}
		else {
			GE_CORE_INFO("Something went wrong updating");
		}
		

	}
};