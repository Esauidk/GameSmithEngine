#include "TopologyResource.h"
namespace Render {
	TopologyResource::TopologyResource(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, D3D12_PRIMITIVE_TOPOLOGY listType) :type(type), listType(listType) {}

	void TopologyResource::Setup(PipelineState& pipeline) {
		pipeline.Attach(type);
	}

	void TopologyResource::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
		cmdList->IASetPrimitiveTopology(listType);
	}
};
