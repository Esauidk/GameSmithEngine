#include "gepch.h"
#include "DirectX12GeometryPack.h"

namespace ProjectGE {
	void DirectX12GeometryPack::Bind()
	{
		m_top->Bind();
		m_vb->Bind();
		m_ib->Bind();
	}

};
