#pragma once
#include "Component.h"
#include "ProjectGE/ResourceAssets/MeshAsset.h"
#include "ProjectGE/Rendering/RenderAgnostics/MaterialSystem/Material.h"

namespace ProjectGE {
	class MeshRenderer : public Component {
	public:
		virtual void OnStart() override {}
		virtual void OnUpdate() override;
		virtual void OnDestroy() override {}

		void SetMesh(Ref<MeshAsset> newMesh) { m_Mesh = newMesh; m_Materials.reserve(m_Mesh->GetSubMeshSize()); }
		void SetMaterial(unsigned int index, Ref<Material> mat) { m_Materials[index] = mat; };
	private:
		Ref<MeshAsset> m_Mesh;
		std::vector<Ref<Material>> m_Materials;
	};
};