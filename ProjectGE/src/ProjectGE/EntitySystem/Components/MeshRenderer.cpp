#include "gepch.h"
#include "MeshRenderer.h"

namespace ProjectGE {
	void MeshRenderer::OnUpdate()
	{
		m_Mesh->SetGraphicsMesh();
	}
};