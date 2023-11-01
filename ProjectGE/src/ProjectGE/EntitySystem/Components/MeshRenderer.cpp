#include "gepch.h"
#include "MeshRenderer.h"

namespace ProjectGE {
	void MeshRenderer::OnUpdate()
	{
		m_Mesh->SetGraphicsMesh();

		unsigned int size = m_Mesh->GetSubMeshSize();
		for (unsigned int i = 0; i < size; i++) {
			m_Materials[i]->ApplyMaterial();
			m_Mesh->DrawMesh(i);
		}
	}
};