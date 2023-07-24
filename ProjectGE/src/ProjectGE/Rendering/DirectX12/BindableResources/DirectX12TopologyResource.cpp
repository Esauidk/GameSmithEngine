#include "gepch.h"
#include "DirectX12TopologyResource.h"
namespace ProjectGE {
	DirectX12TopologyResource::DirectX12TopologyResource(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, D3D12_PRIMITIVE_TOPOLOGY listType) :m_Type(type), m_ListType(listType) {}

	void DirectX12TopologyResource::Setup(DirectX12PipelineState& pipeline) {
		pipeline.Attach(m_Type);
	}

	void DirectX12TopologyResource::Bind(ID3D12GraphicsCommandList6* cmdList) {
		cmdList->IASetPrimitiveTopology(m_ListType);
	}
};
