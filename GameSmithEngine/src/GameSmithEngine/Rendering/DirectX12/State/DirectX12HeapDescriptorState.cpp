#include "gepch.h"
#include "DirectX12HeapDescriptorState.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/Util/DirectX12Macos.h"
#include "GameSmithEngine/Rendering/DirectX12/State/DirectX12StateManager.h"

#include "GameSmithEngine/Core/Log.h"



namespace GameSmith {
	DirectX12HeapDescriptorState::DirectX12HeapDescriptorState(DirectX12QueueType cmdType)
	{
		auto& heapDB = DirectX12Core::GetCore().GetHeapDatabase();
		// TODO: Bring initial descriptor count back to 1 and implement descriptor transfering between previous and new heap
		m_ProductionViewHeap = heapDB.AllocateHeap(4, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		m_ProductionSamplerHeap = heapDB.AllocateHeap(4, DescriptorHeapType::SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		m_ViewHeapSize = 4;
		m_CurrentViewFreeSlot = 0;
		m_ViewOriginSlot = 0;
		
		m_SamplerHeapSize = 0;
		m_CurrentSamplerFreeSlot = 0;
		m_SamplerOriginSlot = 0;

		m_CmdType = cmdType;
	}

	void DirectX12HeapDescriptorState::AttachHeap()
	{
		GE_CORE_ASSERT(m_ProductionViewHeap != nullptr, "There is no view heap current set in the heap descriptor state");
		GE_CORE_ASSERT(m_ProductionSamplerHeap != nullptr, "There is no sampler heap current set in the heap descriptor state");

		auto& core = DirectX12Core::GetCore();
		ID3D12DescriptorHeap* heaps[] = { m_ProductionViewHeap->GetHeap(), m_ProductionSamplerHeap->GetHeap() };


		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetDescriptorHeaps(2, heaps);
		}

	}

	void DirectX12HeapDescriptorState::SetSRV(Stages stage, DirectX12RootSignature& root, SRVStorage& descriptors, UINT numDescriptors, UINT& heapSlot)
	{
		GE_CORE_ASSERT(root.HasSRV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");
		GE_CORE_ASSERT(numDescriptors <= m_ViewHeapSize, "More descriptors are required than available slots in the current heap");
		GE_CORE_ASSERT(numDescriptors + heapSlot <= m_ViewHeapSize, "We're to go past the heap boundary with the selected number of descriptors");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		UINT freeSlot = heapSlot;
		heapSlot += numDescriptors;

		SRVSlotMask& curMask = descriptors.Dirty[stage];
		DirectX12ShaderResourceView* srcDescriptors = descriptors.Views[stage];
		
		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptor = srcDescriptors[i].m_View;
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_ProductionViewHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptor.ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			else {
				device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptySRV.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}

			SRVStorage::SetSlotClean(curMask, i);
		}

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetSRVSlot(stage), m_ProductionViewHeap->GetGPUReference(freeSlot));
		}
	
		
	}

	void DirectX12HeapDescriptorState::SetCBV(Stages stage, DirectX12RootSignature& root, CBVStorage& descriptors, UINT numDescriptors, UINT& heapSlot)
	{
		GE_CORE_ASSERT(root.HasCBV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");
		GE_CORE_ASSERT(numDescriptors <= m_ViewHeapSize, "More descriptors are required than available slots in the current heap");
		GE_CORE_ASSERT(numDescriptors + heapSlot <= m_ViewHeapSize, "We're to go past the heap boundary with the selected number of descriptors");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		CBVSlotMask& curMask = descriptors.Dirty[stage];
		D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptors = descriptors.Descriptors[stage];

		UINT freeSlot = heapSlot;
		heapSlot += numDescriptors;

		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_ProductionViewHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptors[i].ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptors[i], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			else {
				device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptyCBV.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			CBVStorage::SetSlotClean(curMask, i);
		}

		

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetCBVSlot(stage), m_ProductionViewHeap->GetGPUReference(freeSlot));
		}
	}

	void DirectX12HeapDescriptorState::SetUAV(Stages stage, DirectX12RootSignature& root, D3D12_CPU_DESCRIPTOR_HANDLE* descriptors, UINT numDescriptors, UINT& heapSlot)
	{
		GE_CORE_ASSERT(root.HasUAV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxUAV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxUAV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");
		GE_CORE_ASSERT(numDescriptors <= m_ViewHeapSize, "More descriptors are required than available slots in the current heap");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptors = descriptors;


		UINT freeSlot = heapSlot;
		heapSlot += numDescriptors;
		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_ProductionViewHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptors[i].ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptors[i], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			else {
				// TODO: Add UAV default view
				//device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptyCBV.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			//CBVStorage::SetSlotClean(curMask, i);
		}

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetUAVSlot(stage), m_ProductionViewHeap->GetGPUReference(freeSlot));
		}
		
	}

	void DirectX12HeapDescriptorState::SetSampler(Stages stage, DirectX12RootSignature& root, SamplerStorage& descriptors, UINT numDescriptors, UINT& heapSlot)
	{
		GE_CORE_ASSERT(root.HasSamplers(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxSampler(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxSampler(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		SamplerSlotMask& curMask = descriptors.Dirty[stage];
		DirectX12SamplerView* srcDescriptors = descriptors.Views[stage];

		UINT freeSlot = heapSlot;
		heapSlot += numDescriptors;
		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptor = srcDescriptors[i].m_View;
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_ProductionSamplerHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptor.ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			}
			else {
				device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptySampler.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			}

			SamplerStorage::SetSlotClean(curMask, i);
		}



		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetSamplerSlot(stage), m_ProductionSamplerHeap->GetGPUReference(freeSlot));
		}
	}

	bool DirectX12HeapDescriptorState::CanFitView(UINT numDescriptors)
	{
		if (m_ProductionViewHeap.get() == nullptr) {
			return false;
		}

		return m_CurrentViewFreeSlot + numDescriptors <= m_ViewHeapSize;
	}

	bool DirectX12HeapDescriptorState::CanFitSampler(UINT numDescriptors)
	{
		if (m_ProductionSamplerHeap == nullptr) {
			return false;
		}

		return m_CurrentSamplerFreeSlot + numDescriptors <= m_SamplerHeapSize;
	}

	UINT DirectX12HeapDescriptorState::GetFreeViewSlots(UINT requiredDescritpors)
	{
		UINT curFreeSlot = m_CurrentViewFreeSlot;
		m_CurrentViewFreeSlot += requiredDescritpors;

		return curFreeSlot;
	}

	UINT DirectX12HeapDescriptorState::GetFreeSamplerSlots(UINT requiredDescritpors)
	{
		UINT curFreeSlot = m_CurrentSamplerFreeSlot;
		m_CurrentSamplerFreeSlot += requiredDescritpors;

		return curFreeSlot;
	}


	void DirectX12HeapDescriptorState::ReallocateViewHeap(UINT requiredDescriptors)
	{
		auto& core = DirectX12Core::GetCore();
		auto& heapDB = core.GetHeapDatabase();

		if (m_ProductionViewHeap.get() != nullptr) {
			m_ProductionViewHeap->Free();
		}
		
		m_ProductionViewHeap = heapDB.AllocateHeap(requiredDescriptors, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		m_ViewHeapSize = requiredDescriptors;
		m_CurrentViewFreeSlot = 0;

		// TODO: MAKE THIS WORK FOR MORE THAN JUST DIRECT CONTEXT
		core.GetDirectCommandContext().GetStateManager().NewDescriptorHeap();

		AttachHeap();
	}

	void DirectX12HeapDescriptorState::ReallocateSamplerHeap(UINT requiredDescriptors)
	{
		auto& core = DirectX12Core::GetCore();
		auto& heapDB = core.GetHeapDatabase();

		if (m_ProductionSamplerHeap != nullptr) {
			m_ProductionSamplerHeap->Free();
		}

		m_ProductionSamplerHeap = heapDB.AllocateHeap(requiredDescriptors, DescriptorHeapType::SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		m_SamplerHeapSize = requiredDescriptors;
		m_CurrentSamplerFreeSlot = 0;

		// TODO: MAKE THIS WORK FOR MORE THAN JUST DIRECT CONTEXT
		core.GetDirectCommandContext().GetStateManager().NewDescriptorHeap();

		AttachHeap();
	}
};

