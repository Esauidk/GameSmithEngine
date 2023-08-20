#include "gepch.h"
#include "DirectX12PipelineState.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"


namespace ProjectGE {
	void DirectX12PipelineState::Create(DirectX12PipelineArgs& args)
	{
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		D3D12_PIPELINE_STATE_STREAM_DESC stateStreamDesc = { sizeof(args.state), &(args.state) };

		bool res = FAILED(device->CreatePipelineState(&stateStreamDesc, IID_PPV_ARGS(&m_PipelineState)));
		GE_CORE_ASSERT(!res, "Failed to create pipeline state");

	}


	/*void DirectX12PipelineState::Bind() {
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->SetPipelineState(m_PipelineState.Get());
	}*/
};
