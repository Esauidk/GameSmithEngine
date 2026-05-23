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
			unsigned int size = m_Mesh->GetSubMeshSize();
			for (unsigned int i = 0; i < size; i++) {
				if (m_Materials[i].first == nullptr) {
					continue;
				}
				
				renderPrep->AddRenderRequest({m_Mesh->GetVerticies(), m_Mesh->GetSubMesh(i).m_Index, m_Materials[i].first, *transform});
			}
		}
		else {
			GE_CORE_INFO("Something went wrong updating");
		}
		
	}

	void MeshRenderer::SetMaterial(unsigned int index, Ref<MaterialAsset> mat)
	{
		if (index >= m_Materials.size()) {
			GE_CORE_ERROR("Tried to set material at index {0} but mesh only has {1} submeshes", index, m_Materials.size());
			return;
		}

		const auto& existingMat = m_Materials[index];
		if (existingMat.second != mat) {
			m_Materials[index] = { mat->CreateInstance(), mat };
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

		if (m_TestAsset != nullptr) {
			SetMaterial(0, m_TestAsset);
		}
	}
};