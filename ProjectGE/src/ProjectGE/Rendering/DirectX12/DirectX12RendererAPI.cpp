#include "gepch.h"
#include "DirectX12RendererAPI.h"

#include "DirectX12Context.h"

namespace ProjectGE {
	void DirectX12RendererAPI::SetClearColor(const glm::vec4& color)
	{
	}
	void DirectX12RendererAPI::Clear()
	{
	}
	void DirectX12RendererAPI::DrawIndexed(VertexBuffer* vBuffer,IndexBuffer* iBuffer)
	{
		auto& list = DirectX12Context::GetDirectCommandList();
		list->DrawIndexedInstanced(iBuffer->GetCount(), 1, 0, 0, 0);
		DirectX12Context::FinalizeCommandList(DirectX12QueueType::Direct);
	}
};
