#include "gepch.h"
#include "PipelineState.h"
#include "ProjectGE/Log.h"


namespace ProjectGE {
	void PipelineState::Attach(ID3D12RootSignature* rootSig) {
		stateStream.pRootSignature = rootSig;
	}
	void PipelineState::Attach(const D3D12_INPUT_LAYOUT_DESC& layout) {
		stateStream.InputLayout = layout;
	}
	void PipelineState::Attach(const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology) {
		stateStream.PrimitiveTopologyType = topology;
	}
	void PipelineState::Attach(ID3DBlob* shaderByte, const ShaderType type) {
		auto shader = CD3DX12_SHADER_BYTECODE(shaderByte);
		switch (type) {
		case VERTEX:
			stateStream.VS = shader;
			break;
		case PIXEL:
			stateStream.PS = shader;
			break;
		}
	}
	void PipelineState::Attach(const DXGI_FORMAT& format) {
		stateStream.DSVFormat = format;
	}
	void PipelineState::Attach(const D3D12_RT_FORMAT_ARRAY& formats) {
		stateStream.RTVFormats = formats;
	}

	void PipelineState::Build(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice) {
		//TODO: Add checks before building

		D3D12_PIPELINE_STATE_STREAM_DESC stateStreamDesc = {sizeof(stateStream), &stateStream};

		bool res = FAILED(pDevice->CreatePipelineState(&stateStreamDesc, IID_PPV_ARGS(&pPipelineState)));
		GE_CORE_ASSERT(!res, "Failed to create pipeline state");
	}

	void PipelineState::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
		cmdList->SetPipelineState(pPipelineState.Get());
	}
};
