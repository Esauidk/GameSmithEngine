#include "gepch.h"
#include "DirectX12RendererAPI.h"
#include "DirectX12Core.h"

namespace ProjectGE {
	DirectX12RendererAPI::DirectX12RendererAPI(): m_Core(DirectX12Core::CreateCore())
	{}

	void DirectX12RendererAPI::SetClearColor(const glm::vec4& color)
	{
	}
	void DirectX12RendererAPI::Clear()
	{
	}

	void DirectX12RendererAPI::DrawIndexed(Ref<GeometryPack> geopack)
	{
		auto& core = DirectX12Core::GetCore();
		auto& list = core.GetDirectCommandContext().GetCommandList();
		list->DrawIndexedInstanced(geopack->GetIndexBuffer()->GetCount(), 1, 0, 0, 0);
	}

	void DirectX12RendererAPI::FinishRecording()
	{
		auto& core = DirectX12Core::GetCore();
		core.GetDirectCommandContext().FinalizeCommandList();
	}
	Ref<VertexBuffer> DirectX12RendererAPI::CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount)
	{
		return std::make_shared<DirectX12VertexBuffer>(data, vertexByteSize, vertexCount);
	}
	void DirectX12RendererAPI::SetVertexBuffer(Ref<VertexBuffer> vbuffer)
	{
	}
	Ref<IndexBuffer> DirectX12RendererAPI::CreateIndexBuffer(unsigned int* data, unsigned int indexCount)
	{
		return std::make_shared<DirectX12IndexBuffer>(data, indexCount);
	}
	void DirectX12RendererAPI::SetIndexBuffer(Ref<IndexBuffer> ibuffer)
	{
	}
	Ref<Shader> DirectX12RendererAPI::LoadShader()
	{
		return Ref<Shader>();
	}
	void DirectX12RendererAPI::SubmitRecording()
	{
	}
};
