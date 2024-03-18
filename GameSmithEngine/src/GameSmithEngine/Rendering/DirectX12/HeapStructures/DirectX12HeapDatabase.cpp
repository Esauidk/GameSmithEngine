#include "gepch.h"
#include "DirectX12HeapDatabase.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Core/Log.h"

#define DESCRIPTOR_LIMIT 50000

namespace GameSmith {
	D3D12_DESCRIPTOR_HEAP_TYPE ConvertHeapType(DescriptorHeapType heapType) {
		switch (heapType) {
		case DescriptorHeapType::CBVSRVUAV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		case DescriptorHeapType::RT:
			return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case DescriptorHeapType::ST:
			return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		case DescriptorHeapType::SAMPLER:
			return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		default:
			break;
		}

		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	}

	Ref<DirectX12DescriptorHeap> DirectX12HeapDatabase::AllocateHeap(UINT numDescriptor, DescriptorHeapType heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flags, std::string heapName)
	{
		for (Ref<DirectX12DescriptorHeap>& heap : m_AvailableHeaps) {
			if (!heap->IsReserved() && !heap->UsedInFrame() && heap->GetType() == heapType && heap->GetNumDescriptors() >= numDescriptor) {
				heap->Reserve();
				return heap;
			}
		}

		GE_CORE_ASSERT(((m_TotalDescriptorSize + numDescriptor) <= DESCRIPTOR_LIMIT), "Too many descriptors are being used on the GPU, consider optimizing the rate of descriptor assignment");

		ComPtr<ID3D12DescriptorHeap> newHeap;
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = numDescriptor;
		heapDesc.Flags = flags;

		heapDesc.Type = ConvertHeapType(heapType);

		bool res = FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&newHeap)));
		GE_CORE_ASSERT(!res, "Failed to create descriptor heap");

		auto nameConvert = std::wstring(heapName.begin(), heapName.end());
		newHeap->SetName(nameConvert.c_str());

		auto wrappedHeap = Ref<DirectX12DescriptorHeap>(new DirectX12DescriptorHeap(newHeap, numDescriptor, heapType, flags));

		m_AvailableHeaps.emplace_back(wrappedHeap);
		m_TotalDescriptorSize += numDescriptor;

		return wrappedHeap;
	}

	void DirectX12HeapDatabase::FrameEnded()
	{
		for (auto it = m_AvailableHeaps.begin(); it != m_AvailableHeaps.end();) {
			auto& heap = *it;
			if (!heap->IsReserved()) {
				if (heap->UsedInFrame()) {
					heap->EndFrame();
				}
				else {
					if (heap->NumberFramesUnused() <= 2) {
						m_TotalDescriptorSize -= heap->GetNumDescriptors();
						it = m_AvailableHeaps.erase(it);
						continue;
					}
					else {
						heap->IncreamentNotUse();
					}
				}
				
			}

			it++;
		}
	}

	DirectX12DescriptorHeap::DirectX12DescriptorHeap(ComPtr<ID3D12DescriptorHeap> newHeap, UINT numDescriptor, DescriptorHeapType heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flags) : 
		m_CurrentHeap(newHeap),
		m_DescriptorNum(numDescriptor), 
		m_Type(heapType), 
		m_Flags(flags), 
		m_UnitSize(DirectX12Core::GetCore().GetDevice()->GetDescriptorHandleIncrementSize(ConvertHeapType(heapType))),
		m_CpuStartPos(m_CurrentHeap->GetCPUDescriptorHandleForHeapStart()),
		m_GpuStartPos((flags& D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) ? m_CurrentHeap->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE{}),
		m_Reserve(true),
		m_UsedInCurrentFrame(true),
		m_FramesSinceLastUse(0)
	{
	}
};
