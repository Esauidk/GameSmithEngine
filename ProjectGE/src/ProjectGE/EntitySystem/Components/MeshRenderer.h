#pragma once
#include "Component.h"
#include "ProjectGE/ResourceAssets/MeshAsset.h"
#include "ProjectGE/Rendering/RenderAgnostics/MaterialSystem/Material.h"

namespace ProjectGE {
	class MeshRenderer : public Component {
	public:
		MeshRenderer(GameObject* gameObject, Transform* transform) : Component(gameObject, transform) {}
		virtual void OnStart() override {}
		virtual void OnUpdate() override;
		virtual void OnDestroy() override {}

		void SetMesh(Ref<MeshAsset> newMesh) { m_Mesh = newMesh; m_Materials.resize(m_Mesh->GetSubMeshSize()); }
		void SetMaterial(unsigned int index, Ref<Material> mat) { m_Materials[index] = mat; };
		unsigned int GetMaterialSlots() { return m_Mesh->GetSubMeshSize(); }
	private:
		Ref<MeshAsset> m_Mesh;
		std::vector<Ref<Material>> m_Materials;
	};
};