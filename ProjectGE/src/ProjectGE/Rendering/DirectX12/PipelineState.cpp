#include "gepch.h"
#include "PipelineState.h"
#include "ProjectGE/Log.h"


namespace ProjectGE {
	void PipelineState::Attach(ID3D12RootSignature* rootSig) {
		m_StateStream.pRootSignature = rootSig;
	}
	void PipelineState::Attach(const D3D12_INPUT_LAYOUT_DESC& layout) {
		m_StateStream.InputLayout = layout;
	}
	void PipelineState::Attach(const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology) {
		m_StateStream.PrimitiveTopologyType = topology;
	}
	void PipelineState::Attach(ID3DBlob* shaderByte, const ShaderType type) {
		auto shader = CD3DX12_SHADER_BYTECODE(shaderByte);
		switch (type) {
		case VERTEX:
			m_StateStream.VS = shader;
			break;
		case PIXEL:
			m_StateStream.PS = shader;
			break;
		}
	}
	void PipelineState::Attach(const DXGI_FORMAT& format) {
		m_StateStream.DSVFormat = format;
	}
	void PipelineState::Attach(const D3D12_RT_FORMAT_ARRAY& formats) {
		m_StateStream.RTVFormats = formats;
	}

	void PipelineState::SetDebug() {
		m_StateStream.Flags |= D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG;
	}

	void PipelineState::Build(ID3D12Device8* pDevice) {
		//TODO: Add checks before building

		D3D12_PIPELINE_STATE_STREAM_DESC stateStreamDesc = {sizeof(m_StateStream), &m_StateStream};
		
		bool res = FAILED(pDevice->CreatePipelineState(&stateStreamDesc, IID_PPV_ARGS(&m_PipelineState)));
		GE_CORE_ASSERT(!res, "Failed to create pipeline state");
	}

	void PipelineState::Bind(ID3D12GraphicsCommandList6* cmdList) {
		cmdList->SetPipelineState(m_PipelineState.Get());
	}
};
