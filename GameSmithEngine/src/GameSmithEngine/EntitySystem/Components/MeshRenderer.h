#pragma once
#include "Component.h"
#include "GameSmithEngine/ResourceAssets/MeshAsset.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"

namespace GameSmith {
	class MeshRenderer : public Component {
	public:
		MeshRenderer(GameObject* gameObject, Transform* transform) : Component(gameObject, transform) {}
		virtual void OnStart() override {}
		virtual void OnUpdate(float dt) override;
		virtual void OnDestroy() override {}
		virtual int GetPriority() const override { return 0; };
		virtual std::string GetName() const override { return "MeshRenderer"; };

		void SetMesh(Ref<MeshAsset> newMesh) { m_Mesh = newMesh; m_Materials.resize(m_Mesh->GetSubMeshSize()); }
		void SetMaterial(unsigned int index, Ref<Material> mat) { m_Materials[index] = mat; };
		unsigned int GetMaterialSlots() { return m_Mesh->GetSubMeshSize(); }
	private:
		Ref<MeshAsset> m_Mesh;
		std::vector<Ref<Material>> m_Materials;
	};
};