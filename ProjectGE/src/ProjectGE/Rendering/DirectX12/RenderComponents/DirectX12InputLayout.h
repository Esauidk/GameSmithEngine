#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BufferLayout.h"
#include "ProjectGE/Core/Core.h"
#include <d3d12.h>

namespace ProjectGE {
	class DirectX12InputLayout
	{
	public:
		DirectX12InputLayout(const BufferLayoutBuilder& layout);
		inline D3D12_INPUT_LAYOUT_DESC& GetInternalLayout() { return m_InputLayout; }
	private:
		Scope<D3D12_INPUT_ELEMENT_DESC[]> m_Desc;
		D3D12_INPUT_LAYOUT_DESC m_InputLayout;
	};
};


