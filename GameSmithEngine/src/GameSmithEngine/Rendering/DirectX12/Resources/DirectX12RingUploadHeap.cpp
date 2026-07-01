#include "gepch.h"
#include "DirectX12RingUploadHeap.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"

namespace GameSmith {
	DirectX12RingUploadHeap::DirectX12RingUploadHeap()
	{
		auto& core = DirectX12Core::GetCore();
		auto pDevice = core.GetDevice();

		CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(256 * 1024); // 256 KB buffer

		ComPtr<ID3D12Resource2> buffer;
		bool res = FAILED(pDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&buffer)
		));
		GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}", "RingeHeapBuffer");

		m_bufferSize = 256 * 1024; // 256 KB buffer size
		m_Heap = Scope<DirectX12Resource>(new DirectX12Resource(buffer.Get(), D3D12_RESOURCE_STATE_GENERIC_READ));

		CD3DX12_RANGE readRange(0, 0);
		res = m_Heap->GetResource()->Map(0, &readRange, &m_mappedData);
		GE_CORE_ASSERT(!res, "Failed to map GPU Buffer {0}", "RingeHeapBuffer");
	}

	// INVARIANTS: 
	// 1) m_currentOffset is always the next free byte in the buffer
	// 2) m_lifeGroupBoundaries is always sorted from oldest to newest life index
	// 3) m_activeAllocations contains all active allocations with their corresponding life index and offset
	RingUploadAllocation DirectX12RingUploadHeap::Allocate(unsigned int dataSize, unsigned int alignment)
	{
		RingUploadAllocation result;

		GE_CORE_ASSERT(dataSize > 0, "Cannot allocate 0 bytes");
		GE_CORE_ASSERT(dataSize <= m_bufferSize, "Cannot allocate more than the buffer size");

		// Find the oldest allocation with an active allocation
		LifeGroupAllocation oldestAllocation;
		bool existingAllocation = false;
		while (!m_lifeGroupBoundaries.empty()) {
			unsigned int oldestIndex = m_lifeGroupBoundaries.front();
			if (!m_activeAllocations.contains(oldestIndex)) {
				m_lifeGroupBoundaries.pop();
			}
			else {
				oldestAllocation = m_activeAllocations[oldestIndex];
				existingAllocation = true;
				break;
			}
		}


		bool lowerBound = existingAllocation ? oldestAllocation.offset > m_currentOffset : false;
		unsigned int alignedOffset = AlignOffset(m_currentOffset, alignment);

		// Make sure we're still in bounds after alignment
		if (existingAllocation) {
			if (lowerBound) {
				GE_CORE_ASSERT(alignedOffset < oldestAllocation.offset, "Upload heap allocation exceeds buffer bounds");
			}
			else {
				GE_CORE_ASSERT(alignedOffset > oldestAllocation.offset, "Upload heap allocation exceeds buffer bounds");
			}
		}
		

		if (alignedOffset >= m_bufferSize || alignedOffset + dataSize > m_bufferSize) {
			// Loop back to the beginning of the buffer and realign
			alignedOffset = AlignOffset(0, alignment);
			GE_CORE_ASSERT(alignedOffset + dataSize <= m_bufferSize, "Upload heap allocation exceeds buffer bounds");

			if (existingAllocation) {
				lowerBound = true; // If we were previously in the upper bound, we're now in the lower bound

				// Make sure we're still in bounds after wrapping
				GE_CORE_ASSERT(alignedOffset < oldestAllocation.offset, "Upload heap allocation exceeds buffer bounds");
			}
		}

		if (existingAllocation && lowerBound) {
			GE_CORE_ASSERT(alignedOffset + dataSize <= oldestAllocation.offset, "Upload heap allocation exceeds buffer bounds");
		}

		// We have enough space at the end of the buffer, we can allocate here
		result.cpuHandle = static_cast<char*>(m_mappedData) + alignedOffset;
		result.gpuHandle = m_Heap->GetResource()->GetGPUVirtualAddress() + alignedOffset;
		result.size = dataSize;
		result.lifeIndex = m_lifeIndex;

		m_currentOffset = alignedOffset + dataSize;

		if (!m_activeAllocations.contains(m_lifeIndex)) {
			m_lifeGroupBoundaries.push(m_lifeIndex);
			m_activeAllocations[m_lifeIndex] = { alignedOffset, m_lifeIndex };
		}

		return result;
	}

	void DirectX12RingUploadHeap::Flush(unsigned int lifeIndex)
	{
		GE_CORE_ASSERT(m_lifeIndex != lifeIndex, "Cannot flush a life index that is currently being allocated to");
		m_activeAllocations.erase(lifeIndex);
	}

	unsigned int DirectX12RingUploadHeap::IncrementLifeIndex()
	{
		m_lifeIndex++;
		return m_lifeIndex;
	}

	unsigned int DirectX12RingUploadHeap::AlignOffset(unsigned int offset, unsigned int alignment)
	{
		if (offset % alignment == 0) {
			return offset;
		}

		return offset + (offset % alignment);
	}
}