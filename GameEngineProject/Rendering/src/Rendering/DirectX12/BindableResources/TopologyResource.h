#pragma once
#include "PipelineDefiner.h"
namespace Render {
	class TopologyResource : public PipelineDefiner
	{
	public:
		TopologyResource(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, D3D12_PRIMITIVE_TOPOLOGY listType);
		void Setup(PipelineState& pipeline) override;
		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	private:
		D3D12_PRIMITIVE_TOPOLOGY_TYPE type;
		D3D12_PRIMITIVE_TOPOLOGY listType;
	};
};


