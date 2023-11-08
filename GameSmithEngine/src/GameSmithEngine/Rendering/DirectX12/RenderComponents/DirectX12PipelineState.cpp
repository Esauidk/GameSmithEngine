#include "gepch.h"
#include "DirectX12PipelineState.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"


namespace GameSmith {
	void DirectX12PipelineState::Create(DirectX12PipelineArgs& args)
	{
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		D3D12_PIPELINE_STATE_STREAM_DESC stateStreamDesc = { sizeof(args.state), &(args.state) };

		bool res = FAILED(device->CreatePipelineState(&stateStreamDesc, IID_PPV_ARGS(&m_PipelineState)));
		GE_CORE_ASSERT(!res, "Failed to create pipeline state");

	}
};
