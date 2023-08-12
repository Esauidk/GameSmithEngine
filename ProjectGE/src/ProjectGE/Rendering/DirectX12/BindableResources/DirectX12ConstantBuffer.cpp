#include "gepch.h"
#include "DirectX12ConstantBuffer.h"

void ProjectGE::DirectX12ConstantBuffer::UpdateData(BYTE* data)
{
	if (m_State != D3D12_RESOURCE_STATE_COPY_DEST) {
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			this->m_GpuBuffer.Get(),
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, D3D12_RESOURCE_STATE_COPY_DEST
		);

		cmdList->ResourceBarrier(1, &barrier);
		m_State = D3D12_RESOURCE_STATE_COPY_DEST;
	}

	DirectX12Buffer::UpdateData(data);
}

void* ProjectGE::DirectX12ConstantBuffer::GetGPUReference()
{
	if (m_State == D3D12_RESOURCE_STATE_COPY_DEST) {
		DirectX12Context::InitializeQueueWait(DirectX12QueueType::Copy, DirectX12QueueType::Direct, this->m_UploadSignal);

		auto& cmdList = DirectX12Context::GetDirectCommandList();
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			this->m_GpuBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		);
		cmdList->ResourceBarrier(1, &barrier);
		m_State = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	}
	
	
	return &m_GPUAddress;
}
