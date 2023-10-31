#pragma once
#include "Component.h"
#include "ProjectGE/ResourceAssets/MeshAsset.h"

namespace ProjectGE {
	class MeshRenderer : public Component {
	public:
		virtual void OnStart() override {}
		virtual void OnUpdate() override;
		virtual void OnDestroy() override {}

		void SetMesh(Ref<MeshAsset> newMesh) { m_Mesh = newMesh; }
	private:
		Ref<MeshAsset> m_Mesh;
	};
};