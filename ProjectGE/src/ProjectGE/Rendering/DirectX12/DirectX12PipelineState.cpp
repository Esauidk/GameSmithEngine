#include "gepch.h"
#include "DirectX12PipelineState.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"


namespace ProjectGE {
	DirectX12PipelineState::DirectX12PipelineState()
	{
		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_StateStream.RTVFormats = rtvFormats;

		m_StateStream.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	}
	void DirectX12PipelineState::Attach(ID3D12RootSignature* rootSig) {
		m_StateStream.pRootSignature = rootSig;
	}
	void DirectX12PipelineState::Attach(const D3D12_INPUT_LAYOUT_DESC& layout) {
		m_StateStream.InputLayout = layout;
	}
	void DirectX12PipelineState::Attach(const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology) {
		m_StateStream.PrimitiveTopologyType = topology;
	}
	void DirectX12PipelineState::Attach(ID3DBlob* shaderByte, const ShaderType type) {
		auto shader = CD3DX12_SHADER_BYTECODE(shaderByte);
		switch (type) {
		case ShaderType::Vertex:
			m_StateStream.VS = shader;
			break;
		case ShaderType::Pixel:
			m_StateStream.PS = shader;
			break;
		}
	}

	void DirectX12PipelineState::SetDebug() {
		m_StateStream.Flags |= D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG;
	}

	void DirectX12PipelineState::Build() {
		//TODO: Add checks before building

		auto pDevice = DirectX12Context::GetDevice();
		D3D12_PIPELINE_STATE_STREAM_DESC stateStreamDesc = {sizeof(m_StateStream), &m_StateStream};
		
		bool res = FAILED(pDevice->CreatePipelineState(&stateStreamDesc, IID_PPV_ARGS(&m_PipelineState)));
		GE_CORE_ASSERT(!res, "Failed to create pipeline state");
	}

	void DirectX12PipelineState::Bind() {
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->SetPipelineState(m_PipelineState.Get());
	}
};
