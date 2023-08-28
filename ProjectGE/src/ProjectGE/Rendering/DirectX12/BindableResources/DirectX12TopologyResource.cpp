#include "gepch.h"
#include "DirectX12TopologyResource.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

namespace ProjectGE {
	DirectX12TopologyResource::DirectX12TopologyResource(TopologyType type) {
		switch (type) {
		case TopologyType::Triangle:
			{
				m_Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				m_ListType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				break;
			}
		}
	}

	/*void DirectX12TopologyResource::Append(PipelineStateObject& pipeline) {
		auto& dPipeline = (DirectX12PipelineState&)pipeline;
		dPipeline.Attach(m_Type);
	}*/

	void DirectX12TopologyResource::Bind() {
		auto& cmdList = DirectX12Core::GetCore().GetDirectCommandContext().GetCommandList();
		cmdList->IASetPrimitiveTopology(m_ListType);
	}
};
