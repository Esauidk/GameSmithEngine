#include "gepch.h"
#include "DirectX12DirectConstantBuffer.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"

namespace GameSmith {
	DirectX12DirectConstantBuffer::DirectX12DirectConstantBuffer(DirectX12RingUploadHeap& backingHeap, BYTE* data, UINT size) : m_UploadHeap(backingHeap), m_CurrentAllocation{}, m_ViewDescriptor{}, m_Size(size)
	{
		m_ViewDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
	}

	DirectX12DirectConstantBuffer::DirectX12DirectConstantBuffer(DirectX12RingUploadHeap& backingHeap, UINT size) : m_UploadHeap(backingHeap), m_CurrentAllocation{}, m_ViewDescriptor{}, m_Size(size)
	{
		m_ViewDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
	}

	void DirectX12DirectConstantBuffer::UpdateData(BYTE* data, UINT byteSize)
	{
		GE_CORE_ASSERT(byteSize <= m_Size, "Data size exceeds constant buffer size");
		unsigned int alignedSize = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT * ((byteSize / D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT) + 1);
		auto allocation = m_UploadHeap.Allocate(alignedSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
		memcpy(allocation.cpuHandle, data, byteSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = allocation.gpuHandle;
		desc.SizeInBytes = alignedSize;

		DirectX12Core::GetCore().GetDevice()->CreateConstantBufferView(&desc, m_ViewDescriptor);
		if (!m_IsAllocated)
		{
			m_IsAllocated = true;
		}
	}

	D3D12_GPU_VIRTUAL_ADDRESS& DirectX12DirectConstantBuffer::GetGPUReference()
	{
		GE_CORE_ASSERT(m_IsAllocated, "Constant buffer has not been allocated yet. Call UpdateData first.");
		return m_CurrentAllocation.gpuHandle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12DirectConstantBuffer::GetDescriptor()
	{
		return m_ViewDescriptor;
	}
};