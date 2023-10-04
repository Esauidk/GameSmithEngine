#include <gepch.h>
#include "DirectX12DescriptorLoaderHeapManager.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

namespace ProjectGE {

	static UINT GetDescriptorLoaderHeapSize(DescriptorHeapType type) {
		switch (type) {
		case DescriptorHeapType::CBVSRVUAV:
			return 4096;
		case DescriptorHeapType::RT:
			return 256;
		case DescriptorHeapType::ST:
			return 256;
		case DescriptorHeapType::SAMPLER:
			return 64;
		}

		return 0;
	}

	DirectX12DescriptorLoaderManager::DirectX12DescriptorLoaderManager(DescriptorHeapType type) : m_Type(type)
	{
		m_DescriptorSize = DirectX12Core::GetCore().GetDevice()->GetDescriptorHandleIncrementSize(ConvertHeapType(m_Type));
		m_NumDescriptorsPerHeap = GetDescriptorLoaderHeapSize(m_Type);
	}

	DirectX12LoaderDescriptor DirectX12DescriptorLoaderManager::AllocateSlot()
	{
		DirectX12LoaderDescriptor result{};

		if (((int)m_FreeHeaps.size()) == 0) {
			AllocateHeap();
		}

		auto cur = m_FreeHeaps.front();
		result.HeapIndex = cur;

		DirectX12DescriptorLoaderEntry& entry = m_HeapEntries[cur];
		DirectX12DescriptorLoaderAvailableRange& range = entry.m_FreeRange.front();

		result.ptr = range.start;
		range.start += m_DescriptorSize;

		if (range.start == range.end) {
			entry.m_FreeRange.pop_front();

			if (((int)entry.m_FreeRange.size()) == 0) {
				m_FreeHeaps.pop_front();
			}
		}

		return result;
	}

	void DirectX12DescriptorLoaderManager::AllocateHeap()
	{
		auto& core = DirectX12Core::GetCore();
		Ref<DirectX12DescriptorHeap> heap = core.GetHeapDatabase().AllocateHeap(
			m_NumDescriptorsPerHeap,
			m_Type,
			D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			"Resource Heap Loader"
		);

		D3D12_CPU_DESCRIPTOR_HANDLE base = heap->GetCPUReference(0);
		UINT index = (UINT)m_HeapEntries.size();

		m_FreeHeaps.emplace_back(index);
		m_HeapEntries.emplace_back(heap, base, (size_t)(m_DescriptorSize * m_NumDescriptorsPerHeap));
	}
};