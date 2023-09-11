#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12HeapDatabase.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12RootSignature.h"

namespace ProjectGE {
	struct SRVStorage;
	struct CBVStorage;

	class DirectX12HeapDescriptorState
	{
	public:
		DirectX12HeapDescriptorState(DirectX12QueueType cmdType);
		void AttachHeap();
		void SetSRV(Stages stage, DirectX12RootSignature& root, SRVStorage& descriptors, UINT numDescriptors, UINT heapSlot);
		void SetCBV(Stages stage, DirectX12RootSignature& root, CBVStorage& descriptors, UINT numDescriptors, UINT heapSlot);
		void SetUAV(Stages stage, DirectX12RootSignature& root,  D3D12_CPU_DESCRIPTOR_HANDLE* descriptors, UINT numDescriptors, UINT heapSlot);
	private:
		bool CanFit(UINT numDescriptors);
		void Reallocate(UINT requiredDescriptors);
	private:
		Ref<DirectX12DescriptorHeap> m_CurrentHeap;
		UINT m_HeapSize;
		UINT m_CurrentFreeSlot;
		DirectX12QueueType m_CmdType;
	};
};


