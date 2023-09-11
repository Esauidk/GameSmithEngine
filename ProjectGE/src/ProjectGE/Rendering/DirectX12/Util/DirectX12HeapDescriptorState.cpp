#include "gepch.h"
#include "DirectX12HeapDescriptorState.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Macos.h"

#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Rendering/DirectX12/Util/DirectX12StateManager.h"

namespace ProjectGE {
	DirectX12HeapDescriptorState::DirectX12HeapDescriptorState(DirectX12QueueType cmdType)
	{
		auto& heapDB = DirectX12Core::GetCore().GetHeapDatabase();
		m_CurrentHeap = heapDB.AllocateHeap(2, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		m_HeapSize = 2;
		m_CurrentFreeSlot = 0;
		m_CmdType = cmdType;
	}

	void DirectX12HeapDescriptorState::AttachHeap()
	{
		GE_CORE_ASSERT(m_CurrentHeap != nullptr, "There is no heap current set in the heap descriptor state");
		m_CurrentHeap->AttachHeap(m_CmdType);
	}

	void DirectX12HeapDescriptorState::SetSRV(Stages stage, DirectX12RootSignature& root, SRVStorage& descriptors, UINT numDescriptors, UINT heapSlot)
	{
		GE_CORE_ASSERT(root.HasSRV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		UINT freeSlot = heapSlot;
		UINT endSlot = heapSlot + numDescriptors;

		D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_CurrentHeap->GetCPUReference(freeSlot);
		D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptors[MAX_SRV];

		if (!CanFit(numDescriptors)) {
			Reallocate(numDescriptors);
		}
		
		for (UINT i = 0; i < numDescriptors; i++) {
			srcDescriptors[i] = descriptors.Views[stage][i].m_View;
		}

		device->CopyDescriptors(1, &dstDescriptors, &numDescriptors, numDescriptors, srcDescriptors, nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetSRVSlot(stage), m_CurrentHeap->GetGPUReference(freeSlot));
		}
		
		freeSlot += numDescriptors;
		
	}

	void DirectX12HeapDescriptorState::SetCBV(Stages stage, DirectX12RootSignature& root, CBVStorage& descriptors, UINT numDescriptors, UINT heapSlot)
	{
		GE_CORE_ASSERT(root.HasCBV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		CBVSlotMask& curMask = descriptors.Dirty[stage];
		D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptors = descriptors.Descriptors[stage];

		/*if (!CanFit(numDescriptors)) {
			Reallocate(numDescriptors);
		}*/

		UINT freeSlot = heapSlot;

		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_CurrentHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptors[i].ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptors[i], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			else {
				device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptyCBV.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			CBVStorage::SetSlotClean(curMask, i);
		}

		

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetCBVSlot(stage), m_CurrentHeap->GetGPUReference(freeSlot));
		}

		m_CurrentFreeSlot += numDescriptors;
	}

	void DirectX12HeapDescriptorState::SetUAV(Stages stage, DirectX12RootSignature& root, D3D12_CPU_DESCRIPTOR_HANDLE* descriptors, UINT numDescriptors, UINT heapSlot)
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

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetUAVSlot(stage), m_CurrentHeap->GetGPUReference(freeSlot));
		}
		
		freeSlot += numDescriptors;
	}

	bool DirectX12HeapDescriptorState::CanFit(UINT numDescriptors)
	{
		if (m_CurrentHeap == nullptr) {
			return false;
		}

		return m_CurrentFreeSlot + (numDescriptors-1) < m_HeapSize;
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

		AttachHeap();
	}
};

