#pragma once
#include <gepch.h>
#include <d3d12.h>

#include "ProjectGE/Rendering/DirectX12/Util/DirectX12HeapDatabase.h"
#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	struct DirectX12DescriptorLoaderAvailableRange {
		size_t start;
		size_t end;
	};

	struct DirectX12DescriptorLoaderEntry {
		DirectX12DescriptorLoaderEntry(Ref<DirectX12DescriptorHeap> heap, D3D12_CPU_DESCRIPTOR_HANDLE base, size_t numDescriptors) : m_Heap(heap) {
			m_FreeRange.emplace_back(base.ptr, base.ptr + numDescriptors);
		}

		Ref<DirectX12DescriptorHeap> m_Heap;
		std::list<DirectX12DescriptorLoaderAvailableRange> m_FreeRange;
	};

	struct DirectX12LoaderDescriptor : public D3D12_CPU_DESCRIPTOR_HANDLE {
		UINT HeapIndex = 0;
	};

	class DirectX12DescriptorLoaderManager {
	public:
		DirectX12DescriptorLoaderManager(DescriptorHeapType type);
		DirectX12LoaderDescriptor AllocateSlot();
	private:
		void AllocateHeap();
		std::vector<DirectX12DescriptorLoaderEntry> m_HeapEntries;
		std::list<UINT> m_FreeHeaps;
		DescriptorHeapType m_Type;
		UINT m_DescriptorSize;
		UINT m_NumDescriptorsPerHeap;
	};
};
