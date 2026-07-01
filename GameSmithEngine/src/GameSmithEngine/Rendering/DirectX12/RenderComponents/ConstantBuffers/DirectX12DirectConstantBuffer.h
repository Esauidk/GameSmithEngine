#pragma once
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12RingUploadHeap.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/ConstantBuffers/DirectX12ConstantBuffer.h"

namespace GameSmith {
	class DirectX12DirectConstantBuffer : public DirectX12ConstantBuffer
	{
	public:
		DirectX12DirectConstantBuffer(DirectX12RingUploadHeap& backingHeap,BYTE* data, UINT size);
		DirectX12DirectConstantBuffer(DirectX12RingUploadHeap& backingHeap, UINT size);
		void UpdateData(BYTE* data, UINT byteSize) override;
		inline UINT GetBufferSize() override { return m_Size; }
		D3D12_GPU_VIRTUAL_ADDRESS& GetGPUReference();
		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor();
	private:
		DirectX12RingUploadHeap& m_UploadHeap;
		RingUploadAllocation m_CurrentAllocation;
		DirectX12LoaderDescriptor m_ViewDescriptor;
		unsigned int m_Size;
		bool m_IsAllocated = false;
	};
};