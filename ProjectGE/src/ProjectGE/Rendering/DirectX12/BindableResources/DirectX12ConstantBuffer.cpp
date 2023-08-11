#include "gepch.h"
#include "DirectX12ConstantBuffer.h"

void ProjectGE::DirectX12ConstantBuffer::UpdateData(BYTE* data)
{
	DirectX12Buffer::UpdateData(data);
}

void* ProjectGE::DirectX12ConstantBuffer::GetGPUReference()
{
	DirectX12Context::InitializeQueueWait(DirectX12QueueType::Copy, DirectX12QueueType::Direct, this->m_UploadSignal);

	auto& cmdList = DirectX12Context::GetDirectCommandList();
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		this->m_GpuBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	);
	cmdList->ResourceBarrier(1, &barrier);
	
	return &m_GPUAddress;
}
