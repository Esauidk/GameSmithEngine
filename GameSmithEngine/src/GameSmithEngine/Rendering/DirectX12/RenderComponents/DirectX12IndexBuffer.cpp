#include "gepch.h"
#include "DirectX12IndexBuffer.h"

namespace GameSmith {
	DirectX12IndexBuffer::DirectX12IndexBuffer(unsigned int* buffer, unsigned int count) : m_Buffer(Scope<DirectX12Buffer<unsigned int>>(new DirectX12Buffer<unsigned int>(buffer, count, "Index Buffer"))), m_Count(count) {}

	D3D12_INDEX_BUFFER_VIEW DirectX12IndexBuffer::GenerateView()
	{
		auto& core = DirectX12Core::GetCore();
		m_Buffer->SetUploadGPUBlock();
		m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_INDEX_BUFFER, core.GetDirectCommandContext());
		core.GetDirectCommandContext().FinalizeResourceBarriers();

		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = m_Buffer->GetGPUReference();
		view.Format = DXGI_FORMAT_R32_UINT;
		view.SizeInBytes = m_Buffer->GetSize();

		return view;
	}

	UINT DirectX12IndexBuffer::GetCount()
	{
		return m_Count;
	}
};
