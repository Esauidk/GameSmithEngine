#pragma once
#include "Component.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/MaterialAsset.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/MeshAsset.h"

namespace GameSmith {
	class GE_API MeshRenderer : public Component {
	public:
		MeshRenderer(GameObject* gameObject, Transform* transform);
		virtual void OnStart() override {}
		virtual void OnUpdate(float dt) override;
		virtual void OnDestroy() override {}
		virtual int GetPriority() const override { return 0; };
		virtual std::string GetName() const override { return "MeshRenderer"; };

		void SetMesh(Ref<MeshAsset> newMesh) { m_Mesh = newMesh; m_Materials.resize(m_Mesh->GetSubMeshSize()); }
		void SetMaterial(unsigned int index, Ref<MaterialAsset> mat) { m_Materials[index] = { mat->CreateInstance(), mat }; };
		unsigned int GetMaterialSlots() { return m_Mesh->GetSubMeshSize(); }
		void PostRegistryBootstrap() override;

	private:
		Ref<MeshAsset> m_Mesh;
		Ref<MaterialAsset> m_TestAsset;
		std::vector<std::pair<Ref<Material>, Ref<MaterialAsset>>> m_Materials;
	};

	
};