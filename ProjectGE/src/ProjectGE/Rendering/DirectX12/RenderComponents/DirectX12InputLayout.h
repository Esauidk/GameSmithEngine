#pragma once
#include <d3d12.h>

#include "ProjectGE/Core/Core.h"

#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BufferLayout.h"

namespace ProjectGE {
	class DirectX12InputLayout : public PipelineDefiner
	{
	public:
		DirectX12InputLayout(const BufferLayoutBuilder& layout);
		inline D3D12_INPUT_LAYOUT_DESC& GetInternalLayout() { return m_InputLayout; }
		//void Append(PipelineStateObject& pipeline) override;
	private:
		Scope<D3D12_INPUT_ELEMENT_DESC[]> m_Desc;
		D3D12_INPUT_LAYOUT_DESC m_InputLayout;
	};
};

