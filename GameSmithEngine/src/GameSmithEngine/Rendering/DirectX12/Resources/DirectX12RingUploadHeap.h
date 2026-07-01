#pragma once

#include "gepch.h"
#include "DirectX12Resource.h"
#include "GameSmithEngine/Core/Core.h"
namespace GameSmith {
	struct RingUploadAllocation {
		void* cpuHandle;
		D3D12_GPU_VIRTUAL_ADDRESS gpuHandle;
		unsigned int size = 0;
		unsigned int lifeIndex;
	};

	// TODO:
	// Implement allocation method, needs to pick the next slot(s) in the buffer (if available) and return an allocation
	// WE NEED TO HANDLE WRAPPING PROPERLY (consider a secondary output parameter to hold wrapped space data)
	// Implement flush method, grab all known allocations with the specific inidice and mark them as free (no need to sanitize the data)
	// Add a data structure for tracking allocations tied to a life index, this should be used in the flush method to know which allocations to free
	// BE AWARE THAT FRAGMENTATION IS POSSIBLE WHEN ALLOWING ALLOCATION OF DIFFERENT ALIGNMENT REQUIREMENTS (starting address requirements & size requirements)
	// Add a lifeIndex increment method, this should be used when all allocations tied to a life index are complete (moving the buffer to a new life index)

	class DirectX12RingUploadHeap
	{
	public:
		DirectX12RingUploadHeap();
		~DirectX12RingUploadHeap() = default;
		DirectX12RingUploadHeap(const DirectX12RingUploadHeap&) = delete;

		RingUploadAllocation Allocate(unsigned int dataSize, unsigned int alignment);
		void Flush(unsigned int lifeIndex);
		unsigned int IncrementLifeIndex();
	private:
		static unsigned int AlignOffset(unsigned int offset, unsigned int alignment);
	private:
		struct LifeGroupAllocation {
			unsigned int offset;
			unsigned int index; // Totally informational
		};
	private:
		Scope<DirectX12Resource> m_Heap;
		void* m_mappedData = nullptr;

		unsigned int m_currentOffset = 0;
		unsigned int m_lifeIndex = 0;
		unsigned int m_bufferSize = 0;
		
		std::unordered_map<unsigned int, LifeGroupAllocation> m_activeAllocations;
		std::queue<unsigned int> m_lifeGroupBoundaries;
	};
};