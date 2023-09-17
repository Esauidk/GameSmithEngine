#pragma once
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12RootSignature.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12Shader.h"
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12TopologyResource.h"

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

	class DirectX12PipelineStateData {
	public:
		DirectX12PipelineStateData(Ref<DirectX12PipelineState> pso, Ref<DirectX12RootSignature> root) : m_Pso(pso), m_Root(root) {}
	public:
		const Ref<DirectX12PipelineState> m_Pso;
		const Ref<DirectX12RootSignature> m_Root;
	};
};


