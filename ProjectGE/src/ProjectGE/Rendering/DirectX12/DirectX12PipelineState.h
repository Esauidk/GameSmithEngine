#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineStateObject.h"
#include "Util/d3dx12.h"

namespace ProjectGE {
	struct DirectX12PipelineStateStream {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		CD3DX12_PIPELINE_STATE_STREAM_FLAGS Flags;
	};

	enum DirectX12ShaderType {
		VERTEX,
		PIXEL
	};

	class DirectX12PipelineState : public PipelineStateObject
	{
	public:
		DirectX12PipelineState() = default;
		void Attach(ID3D12RootSignature* rootSig);
		void Attach(const D3D12_INPUT_LAYOUT_DESC& layout);
		void Attach(const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology);
		void Attach(ID3DBlob* shaderByte, const DirectX12ShaderType type);
		void Attach(const DXGI_FORMAT& format);
		void Attach(const D3D12_RT_FORMAT_ARRAY& formats);
		void Build() override;
		void SetDebug();
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	private:
		DirectX12PipelineStateStream m_StateStream;
		ComPtr<ID3D12PipelineState> m_PipelineState;
	};
};


