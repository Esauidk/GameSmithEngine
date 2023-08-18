#include "gepch.h"
#include "DirectX12HeapManager.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	static D3D12_DESCRIPTOR_HEAP_TYPE ConvertHeapType(DescriptorHeapType heapType) {
		switch (heapType) {
		case DescriptorHeapType::CBVSRVUAV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		case DescriptorHeapType::RT:
			return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case DescriptorHeapType::ST:
			return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		}

		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	}

	Ref<DirectX12DescriptorHeap> DirectX12HeapManager::AllocateHeap(UINT numDescriptor, DescriptorHeapType heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		for (Ref<DirectX12DescriptorHeap>& heap : m_AvailableHeaps) {
			if (!heap->IsReserved() && heap->GetType() == heapType && heap->GetNumDescriptors() >= numDescriptor) {
				return heap;
			}
		}

		ComPtr<ID3D12DescriptorHeap> newHeap;
		auto device = DirectX12Context::GetDevice();

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = numDescriptor;
		heapDesc.Flags = flags;

		heapDesc.Type = ConvertHeapType(heapType);

		bool res = FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&newHeap)));
		GE_CORE_ASSERT(!res, "Failed to create descriptor heap");

		auto wrappedHeap = Ref<DirectX12DescriptorHeap>(new DirectX12DescriptorHeap(newHeap, numDescriptor, heapType, flags));

		m_AvailableHeaps.emplace_back(wrappedHeap);

		return wrappedHeap;
	}

	DirectX12DescriptorHeap::DirectX12DescriptorHeap(ComPtr<ID3D12DescriptorHeap> newHeap, UINT numDescriptor, DescriptorHeapType heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flags) : 
		m_CurrentHeap(newHeap),
		m_DescriptorNum(numDescriptor), 
		m_Type(heapType), 
		m_Flags(flags), 
		m_UnitSize(DirectX12Context::GetDevice()->GetDescriptorHandleIncrementSize(ConvertHeapType(heapType))),
		m_CpuStartPos(m_CurrentHeap->GetCPUDescriptorHandleForHeapStart()),
		m_GpuStartPos(m_CurrentHeap->GetGPUDescriptorHandleForHeapStart()),
		m_Reserve(false)
	{
	}
};
