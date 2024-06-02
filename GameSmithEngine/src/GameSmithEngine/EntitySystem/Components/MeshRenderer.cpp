#include "gepch.h"
#include "MeshRenderer.h"
#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

namespace GameSmith {
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
	Ref<char> MeshRenderer::Serialize()
	{
		ResourceAssetWriter writer(RequireSpace());
		auto meshID = m_Mesh->GetId().getData();
		writer.WriteClass<idData>(&meshID);
		for (auto mat : m_Materials) {
			auto matID = mat.second->GetId().getData();
			writer.WriteClass<idData>(&matID);
		}

		m_Registry.Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());

		return writer.GetBuffer();
	}

	void MeshRenderer::Serialize(char* byteStream, unsigned int availableBytes)
	{
	}

	unsigned int MeshRenderer::RequireSpace() const
	{
		unsigned int size = 0;
		// The mesh asset id
		size += sizeof(idData);
		// The material asset iDs
		size += ((unsigned int)m_Materials.size()) * sizeof(idData);
		
		size += m_Registry.RequireSpace();

		return size;
	}

	void MeshRenderer::Deserialize(char* inData, unsigned int size)
	{
		ResourceAssetReader reader(inData, size);

		auto meshID = reader.ReadClass<idData>();
		auto resourceManager = ResourceManager::GetInstance();

		auto meshAsset = resourceManager->GetResource<MeshAsset>(ID(*meshID));
		SetMesh(meshAsset);

		int i = 0;
		while (i < m_Materials.size()) {
			auto matID = reader.ReadClass<idData>();
			auto materialAsset = resourceManager->GetResource<MaterialAsset>(ID(*matID));
			SetMaterial(i, materialAsset);
			i++;
		}

		m_Registry.Deserialize(reader.GetCurPtr(), reader.GetRemainingBytes());
	}
};