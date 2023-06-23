#include "gepch.h"
#include "TopologyResource.h"
namespace ProjectGE {
	TopologyResource::TopologyResource(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, D3D12_PRIMITIVE_TOPOLOGY listType) :m_Type(type), m_ListType(listType) {}

	void TopologyResource::Setup(PipelineState& pipeline) {
		pipeline.Attach(m_Type);
	}

	void TopologyResource::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
		cmdList->IASetPrimitiveTopology(m_ListType);
	}
};
