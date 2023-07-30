#pragma once
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Topology.h"

namespace ProjectGE {
	class DirectX12TopologyResource : public Topology
	{
	public:
		DirectX12TopologyResource(TopologyType type);
		void Append(PipelineStateObject& pipeline) override;
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	private:
		D3D12_PRIMITIVE_TOPOLOGY_TYPE m_Type;
		D3D12_PRIMITIVE_TOPOLOGY m_ListType;
	};
};


