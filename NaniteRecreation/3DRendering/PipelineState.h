#pragma once
#include "BindableResource.h"
#include "d3dx12.h"

namespace Render {
	struct PipelineStateStream {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
	};

	enum ShaderType {
		VERTEX,
		PIXEL
	};

	class PipelineState : public BindableResource
	{
	public:
		PipelineState() = default;
		void Attach(ID3D12RootSignature* rootSig);
		void Attach(D3D12_INPUT_LAYOUT_DESC layout);
		void Attach(D3D12_PRIMITIVE_TOPOLOGY_TYPE topology);
		void Attach(ID3DBlob* shaderByte, ShaderType type);
		void Attach(DXGI_FORMAT format);
		void Attach(D3D12_RT_FORMAT_ARRAY formats);
		void Build(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice);
		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	private:
		PipelineStateStream stateStream;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pPipelineState;
	};
};


