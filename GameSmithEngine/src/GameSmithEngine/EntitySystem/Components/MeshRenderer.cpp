#include "gepch.h"
#include "MeshRenderer.h"
#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/EntitySystem/Components/ComponentFactory.h"

namespace GameSmith {
	GE_REGISTERCOMPONENT(MeshRenderer)

	MeshRenderer::MeshRenderer(GameObject* gameObject, Transform* transform) : Component(gameObject, transform)
	{
		m_Registry.AddExposedAsset<MeshAsset>("Mesh", (Ref<GameSmith::IAsset>*)&m_Mesh, CLASS_TO_STRING(MeshAsset));
		m_Registry.AddExposedAsset<MaterialAsset>("TestMat", (Ref<GameSmith::IAsset>*) &m_TestAsset, CLASS_TO_STRING(MaterialAsset));
	}

	void MeshRenderer::OnUpdate(float dt)
	{
		auto renderPrep = EntityRenderPreparer::GetInstance();

		if (renderPrep != nullptr && m_Mesh != nullptr) {
			auto transform = GetTransform();
			glm::mat4 model = transform->GetModelMatrix();
			bool changed = transform->HasChanged();
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

	void MeshRenderer::PostRegistryBootstrap()
	{
		if (m_Mesh != nullptr) {
			unsigned subMeshSize = m_Mesh->GetSubMeshSize();
			if (subMeshSize != m_Materials.size()) {
				m_Materials.resize(subMeshSize);
			}
		}
	}
};