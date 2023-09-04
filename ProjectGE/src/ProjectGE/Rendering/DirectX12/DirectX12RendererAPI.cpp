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
	Ref<Shader> DirectX12RendererAPI::LoadShader()
	{
		return Ref<Shader>();
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
