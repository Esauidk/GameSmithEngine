#include "gepch.h"
#include "DirectX12HeapDescriptorState.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Macos.h"

#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Rendering/DirectX12/State/DirectX12StateManager.h"

namespace ProjectGE {
	DirectX12HeapDescriptorState::DirectX12HeapDescriptorState(DirectX12QueueType cmdType)
	{
		auto& heapDB = DirectX12Core::GetCore().GetHeapDatabase();
		// TODO: Bring initial descriptor count back to 1 and implement descriptor transfering between previous and new heap
		m_CurrentViewHeap = heapDB.AllocateHeap(4, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		m_CurrentSamplerHeap = heapDB.AllocateHeap(4, DescriptorHeapType::SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
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
		GE_CORE_ASSERT(m_CurrentViewHeap != nullptr, "There is no view heap current set in the heap descriptor state");
		GE_CORE_ASSERT(m_CurrentSamplerHeap != nullptr, "There is no sampler heap current set in the heap descriptor state");

		auto& core = DirectX12Core::GetCore();
		ID3D12DescriptorHeap* heaps[] = { m_CurrentViewHeap->GetHeap(), m_CurrentSamplerHeap->GetHeap() };


		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetDescriptorHeaps(2, heaps);
		}

	}

	void DirectX12HeapDescriptorState::SetSRV(Stages stage, DirectX12RootSignature& root, SRVStorage& descriptors, UINT numDescriptors, UINT& heapSlot)
	{
		GE_CORE_ASSERT(root.HasSRV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxSRV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		UINT freeSlot = heapSlot;
		heapSlot += numDescriptors;

		SRVSlotMask& curMask = descriptors.Dirty[stage];
		DirectX12ShaderResourceView* srcDescriptors = descriptors.Views[stage];
		
		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptor = srcDescriptors[i].m_View;
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_CurrentViewHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptor.ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			else {
				device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptySRV.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}

			SRVStorage::SetSlotClean(curMask, i);
		}

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetSRVSlot(stage), m_CurrentViewHeap->GetGPUReference(freeSlot));
		}
	
		
	}

	void DirectX12HeapDescriptorState::SetCBV(Stages stage, DirectX12RootSignature& root, CBVStorage& descriptors, UINT numDescriptors, UINT& heapSlot)
	{
		GE_CORE_ASSERT(root.HasCBV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxCBV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		CBVSlotMask& curMask = descriptors.Dirty[stage];
		D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptors = descriptors.Descriptors[stage];

		UINT freeSlot = heapSlot;
		heapSlot += numDescriptors;
		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_CurrentViewHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptors[i].ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptors[i], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			else {
				device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptyCBV.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			}
			CBVStorage::SetSlotClean(curMask, i);
		}

		

		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetCBVSlot(stage), m_CurrentViewHeap->GetGPUReference(freeSlot));
		}
	}

	void DirectX12HeapDescriptorState::SetUAV(Stages stage, DirectX12RootSignature& root, D3D12_CPU_DESCRIPTOR_HANDLE* descriptors, UINT numDescriptors, UINT& heapSlot)
	{
		GE_CORE_ASSERT(root.HasUAV(), "This root signature has no shader resource views");
		GE_CORE_ASSERT(root.GetMaxUAV(stage) > 0, "This root signature has no shader resource views for this stage");
		GE_CORE_ASSERT(root.GetMaxUAV(stage) >= numDescriptors, "This root signature does not enough descriptors set for this table");

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptors = descriptors;


		UINT freeSlot = heapSlot;
		heapSlot += numDescriptors;
		for (UINT i = 0; i < numDescriptors; i++) {
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_CurrentViewHeap->GetCPUReference(freeSlot + i);

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
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetUAVSlot(stage), m_CurrentViewHeap->GetGPUReference(freeSlot));
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
			D3D12_CPU_DESCRIPTOR_HANDLE dstDescriptors = m_CurrentSamplerHeap->GetCPUReference(freeSlot + i);

			if (srcDescriptor.ptr != 0) {
				device->CopyDescriptorsSimple(1, dstDescriptors, srcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			}
			else {
				device->CopyDescriptorsSimple(1, dstDescriptors, DirectX12Core::GetCore().GetDefaultViews().EmptySampler.m_View, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			}

			SamplerStorage::SetSlotClean(curMask, i);
		}



		if (m_CmdType == DirectX12QueueType::Direct) {
			core.GetDirectCommandContext().GetCommandList()->SetGraphicsRootDescriptorTable(root.GetSamplerSlot(stage), m_CurrentSamplerHeap->GetGPUReference(freeSlot));
		}
	}

	bool DirectX12HeapDescriptorState::CanFitView(UINT numDescriptors)
	{
		if (m_CurrentViewHeap == nullptr) {
			return false;
		}

		return m_CurrentViewFreeSlot + numDescriptors <= m_ViewHeapSize;
	}

	bool DirectX12HeapDescriptorState::CanFitSampler(UINT numDescriptors)
	{
		if (m_CurrentSamplerHeap == nullptr) {
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

		if (m_CurrentViewHeap != nullptr) {
			m_CurrentViewHeap->Free();
		}
		
		m_CurrentViewHeap = heapDB.AllocateHeap(requiredDescriptors, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		m_ViewHeapSize = requiredDescriptors;
		m_CurrentViewFreeSlot = 0;

		// TODO: MAKE THIS WORK FOR MORE THAN JUST DIRECT CONTEXT
		core.GetDirectCommandContext().GetStateManager().NewDescriptorHeap();

		AttachHeap();
	}

	void DirectX12HeapDescriptorState::ReallocateSamplerHeap(UINT requiredDescriptors)
	{
		// TODO: Implement Logic
		auto& core = DirectX12Core::GetCore();
		auto& heapDB = core.GetHeapDatabase();

		if (m_CurrentSamplerHeap != nullptr) {
			m_CurrentSamplerHeap->Free();
		}

		m_CurrentSamplerHeap = heapDB.AllocateHeap(requiredDescriptors, DescriptorHeapType::SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		m_SamplerHeapSize = requiredDescriptors;
		m_CurrentSamplerFreeSlot = 0;

		// TODO: MAKE THIS WORK FOR MORE THAN JUST DIRECT CONTEXT
		core.GetDirectCommandContext().GetStateManager().NewDescriptorHeap();

		AttachHeap();
	}
};

