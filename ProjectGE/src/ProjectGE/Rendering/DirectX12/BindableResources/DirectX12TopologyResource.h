#pragma once
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"

namespace ProjectGE {
	class DirectX12TopologyResource : public PipelineDefiner, public BindableResource
	{
	public:
		DirectX12TopologyResource(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, D3D12_PRIMITIVE_TOPOLOGY listType);
		void Append(PipelineStateObject& pipeline) override;
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	private:
		D3D12_PRIMITIVE_TOPOLOGY_TYPE m_Type;
		D3D12_PRIMITIVE_TOPOLOGY m_ListType;
	};
};


