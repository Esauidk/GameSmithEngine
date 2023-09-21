#include "gepch.h"
#include "DirectX12RendererAPI.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Util.h"

namespace ProjectGE {
	DirectX12RendererAPI::DirectX12RendererAPI(): m_Core(DirectX12Core::CreateCore())
	{}

	void DirectX12RendererAPI::SetClearColor(const glm::vec4& color)
	{
	}
	void DirectX12RendererAPI::Clear()
	{
	}

	void DirectX12RendererAPI::DrawIndexed(UINT indecies, UINT instances)
	{
		
		auto& core = ProjectGE::DirectX12Core::GetCore();
		auto& context = core.GetDirectCommandContext();
		auto& list = context.GetCommandList();
		auto& state = context.GetStateManager();

		state.BindState();
		list->DrawIndexedInstanced(indecies, instances, 0, 0, 0);
	}

	Ref<VertexBuffer> DirectX12RendererAPI::CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount)
	{
		return std::make_shared<DirectX12VertexBuffer>(data, vertexByteSize, vertexCount);
	}

	void DirectX12RendererAPI::SetVertexBuffer(Ref<VertexBuffer> vbuffer)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto dx12Buf = std::dynamic_pointer_cast<DirectX12VertexBuffer>(vbuffer);
		D3D12_VERTEX_BUFFER_VIEW view = dx12Buf->GenerateView();
		context.GetStateManager().SetVBV(view);
	}

	Ref<IndexBuffer> DirectX12RendererAPI::CreateIndexBuffer(unsigned short* data, unsigned int indexCount)
	{
		return std::make_shared<DirectX12IndexBuffer>(data, indexCount);
	}

	void DirectX12RendererAPI::SetIndexBuffer(Ref<IndexBuffer> ibuffer)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto dx12Buf = std::dynamic_pointer_cast<DirectX12IndexBuffer>(ibuffer);
		D3D12_INDEX_BUFFER_VIEW view = dx12Buf->GenerateView();
		context.GetStateManager().SetIBV(view);
	}
	Ref<Shader> DirectX12RendererAPI::LoadShader(std::string path)
	{
		return std::make_shared<DirectX12Shader>(path);
	}

	Ref<ConstantBuffer> DirectX12RendererAPI::CreateConstantBuffer(UINT size)
	{
		return std::make_shared<DirectX12ConstantBuffer>(size);
	}

	void DirectX12RendererAPI::SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto dx12Buf = std::dynamic_pointer_cast<DirectX12ConstantBuffer>(cbuffer);
		D3D12_GPU_VIRTUAL_ADDRESS add = dx12Buf->GetGPUReference();
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor = dx12Buf->GetDescriptor();

		DirectX12ConstantBufferView view;
		view.m_View = descriptor;
		view.m_GPUAdd = add;

		UINT index;

		if (constantType == ShaderConstantType::Global) {
			index = 0;
		}
		else {
			index = 1;
		}

		context.GetStateManager().SetCBV(stage, view, index);
	}

	Ref<Texture2D> DirectX12RendererAPI::CreateTexture2D(std::string& texturePath)
	{
		return std::make_shared<DirectX12Texture2D>(texturePath);
	}

	void DirectX12RendererAPI::SetTexture2D(Ref<Texture2D> tex, Stages stage)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto castTex2d = std::dynamic_pointer_cast<DirectX12Texture2D>(tex);
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor = castTex2d->GetDescriptor();

		DirectX12ShaderResourceView view;
		view.m_View = descriptor;

		// TODO: Define way to set the index
		context.GetStateManager().SetSRV(stage, view, 0);
	}

	Ref<Sampler> DirectX12RendererAPI::CreateSampler(FilterType img, PaddingMethod padMode)
	{
		return std::make_shared<DirectX12Sampler>(img, padMode);
	}

	void DirectX12RendererAPI::SetSampler(Ref<Sampler> sampler, Stages stage)
	{
		// TODO: Get Rid of Test code
		auto& context = m_Core.GetDirectCommandContext();
		auto castSampler = std::dynamic_pointer_cast<DirectX12Sampler>(sampler);
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor = castSampler->GetDescriptor();

		DirectX12SamplerView view;
		view.m_View = descriptor;

		// TODO: Define way to set the index
		context.GetStateManager().SetSampler(stage, view, 0);
	}

	void DirectX12RendererAPI::SetTopology(TopologyType& type)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto& state = context.GetStateManager();
		D3D12_PRIMITIVE_TOPOLOGY d3Type = TranslateTopListType(TopologyType::Triangle);
		state.SetTop(d3Type);
	}

	void DirectX12RendererAPI::SubmitRecording()
	{
		m_Core.GetDirectCommandContext().FinalizeCommandList();
	}
};
