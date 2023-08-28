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
};
