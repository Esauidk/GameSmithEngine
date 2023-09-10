#include "gepch.h"
#include "DirectX12HeapDescriptorState.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Macos.h"

#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Rendering/DirectX12/Util/DirectX12StateManager.h"

namespace ProjectGE {
	void DirectX12HeapDescriptorState::AttachHeap()
	{
		GE_CORE_ASSERT(m_CurrentHeap != nullptr, "There is no heap current set in the heap descriptor state");
		m_CurrentHeap->AttachHeap();
	}

	void DirectX12HeapDescriptorState::SetSRV(Stages stage, DirectX12RootSignature& root, SRVStorage& descriptors, UINT numDescriptors)
	{
		GE_CORE_ASSERT(root.HasSRV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		UINT freeSlot = m_CurrentFreeSlot;

		D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_CurrentHeap->GetCPUReference(freeSlot);
		D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptors[MAX_SRV];

		if (!CanFit(numDescriptors)) {
			Reallocate(numDescriptors);
		}
		
		for (UINT i = 0; i < numDescriptors; i++) {
			srcDescriptors[i] = descriptors.Views[stage][i].m_View;
		}

		device->CopyDescriptors(1, &dstDescriptors, &numDescriptors, numDescriptors, srcDescriptors, nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetSRVSlot(stage), m_CurrentHeap->GetGPUReference(freeSlot));

		freeSlot += numDescriptors;
		
	}

	void DirectX12HeapDescriptorState::SetCBV(Stages stage, DirectX12RootSignature& root, D3D12_CPU_DESCRIPTOR_HANDLE* descriptors, UINT numDescriptors)
	{
		GE_CORE_ASSERT(root.HasCBV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors[MAX_CBV];
		D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptors = descriptors;

		if (!CanFit(numDescriptors)) {
			Reallocate(numDescriptors);
		}

		UINT freeSlot = m_CurrentFreeSlot;
		for (UINT i = 0; i < numDescriptors; i++) {
			dstDescriptors[i] = m_CurrentHeap->GetCPUReference(freeSlot + i);
		}

		device->CopyDescriptors(1, dstDescriptors, &numDescriptors, 1, srcDescriptors, &numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetCBVSlot(stage), m_CurrentHeap->GetGPUReference(freeSlot));

		freeSlot += numDescriptors;
	}

	void DirectX12HeapDescriptorState::SetUAV(Stages stage, DirectX12RootSignature& root, D3D12_CPU_DESCRIPTOR_HANDLE* descriptors, UINT numDescriptors)
	{
		GE_CORE_ASSERT(root.HasUAV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxUAV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxUAV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors[MAX_UAV];
		D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptors = descriptors;

		if (!CanFit(numDescriptors)) {
			Reallocate(numDescriptors);
		}

		UINT freeSlot = m_CurrentFreeSlot;
		for (UINT i = 0; i < numDescriptors; i++) {
			dstDescriptors[i] = m_CurrentHeap->GetCPUReference(freeSlot + i);
		}

		device->CopyDescriptors(1, dstDescriptors, &numDescriptors, numDescriptors, srcDescriptors, nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetUAVSlot(stage), m_CurrentHeap->GetGPUReference(freeSlot));

		freeSlot += numDescriptors;
	}

	bool DirectX12HeapDescriptorState::CanFit(UINT numDescriptors)
	{
		if (m_CurrentHeap == nullptr) {
			return false;
		}

		return m_CurrentFreeSlot + numDescriptors < m_HeapSize;
	}


	void DirectX12HeapDescriptorState::Reallocate(UINT requiredDescriptors)
	{
		auto& heapDB = DirectX12Core::GetCore().GetHeapDatabase();

		if (m_CurrentHeap != nullptr) {
			m_CurrentHeap->Free();
		}
		
		m_CurrentHeap = heapDB.AllocateHeap(m_HeapSize + requiredDescriptors, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		m_HeapSize += requiredDescriptors;

		//TODO: Copy data from old heap to new heap
	}
};

