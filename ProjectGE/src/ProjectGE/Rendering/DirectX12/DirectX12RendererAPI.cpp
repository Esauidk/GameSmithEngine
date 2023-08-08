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
	void DirectX12RendererAPI::DrawIndexed(GeometryPack* geopack)
	{
		auto& list = DirectX12Context::GetDirectCommandList();
		list->DrawIndexedInstanced(geopack->GetIndexBuffer()->GetCount(), 1, 0, 0, 0);
	}
	void DirectX12RendererAPI::FinishRecording()
	{
		DirectX12Context::FinalizeCommandList(DirectX12QueueType::Direct);
	}
};
