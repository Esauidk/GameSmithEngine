#include "PipelineState.h"
#include "DirectXMacros.h"


namespace Render {
	void PipelineState::Attach(ID3D12RootSignature* rootSig) {
		stateStream.pRootSignature = rootSig;
	}
	void PipelineState::Attach(D3D12_INPUT_LAYOUT_DESC layout) {
		stateStream.InputLayout = layout;
	}
	void PipelineState::Attach(D3D12_PRIMITIVE_TOPOLOGY_TYPE topology) {
		stateStream.PrimitiveTopologyType = topology;
	}
	void PipelineState::Attach(CD3DX12_SHADER_BYTECODE shaderByte, ShaderType type) {
		switch (type) {
		case VERTEX:
			stateStream.VS = shaderByte;
			break;
		case PIXEL:
			stateStream.PS = shaderByte;
			break;
		}
	}
	void PipelineState::Attach(DXGI_FORMAT format) {
		stateStream.DSVFormat = format;
	}
	void PipelineState::Attach(D3D12_RT_FORMAT_ARRAY formats) {
		stateStream.RTVFormats = formats;
	}

	void PipelineState::Build(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice) {
		//TODO: Add checks before building

		D3D12_PIPELINE_STATE_STREAM_DESC stateStreamDesc = {sizeof(stateStream), &stateStream};

		RENDER_THROW(pDevice->CreatePipelineState(&stateStreamDesc, IID_PPV_ARGS(&pPipelineState)));
	}
};
