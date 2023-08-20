#pragma once
#include "Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12RootSignature.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Shader.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12TopologyResource.h"

using Microsoft::WRL::ComPtr;

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

	struct DirectX12PipelineArgs {
		DirectX12PipelineStateStream state;
	};

	class DirectX12PipelineState
	{
	public:
		DirectX12PipelineState() = default;
		void Create(DirectX12PipelineArgs& args);
		inline ID3D12PipelineState* GetPipelineState() { return m_PipelineState.Get(); }
		//void Bind() override;
	private:
		ComPtr<ID3D12PipelineState> m_PipelineState;
	};
};


