#pragma once
#include "DirectX12PipelineDefiner.h"
namespace ProjectGE {
	class DirectX12TopologyResource : public DirectX12PipelineDefiner
	{
	public:
		DirectX12TopologyResource(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, D3D12_PRIMITIVE_TOPOLOGY listType);
		void Setup(DirectX12PipelineState& pipeline) override;
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	private:
		D3D12_PRIMITIVE_TOPOLOGY_TYPE m_Type;
		D3D12_PRIMITIVE_TOPOLOGY m_ListType;
	};
};


