#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/DirectX12/HeapStructures/DirectX12HeapDatabase.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12RootSignature.h"

namespace ProjectGE {
	struct SRVStorage;
	struct CBVStorage;
	struct SamplerStorage;

	class DirectX12HeapDescriptorState
	{
	public:
		DirectX12HeapDescriptorState(DirectX12QueueType cmdType);
		void AttachHeap();

		void SetSRV(Stages stage, DirectX12RootSignature& root, SRVStorage& descriptors, UINT numDescriptors, UINT& heapSlot);
		void SetCBV(Stages stage, DirectX12RootSignature& root, CBVStorage& descriptors, UINT numDescriptors, UINT& heapSlot);
		void SetUAV(Stages stage, DirectX12RootSignature& root,  D3D12_CPU_DESCRIPTOR_HANDLE* descriptors, UINT numDescriptors, UINT& heapSlot);
		void SetSampler(Stages stage, DirectX12RootSignature& root, SamplerStorage& descriptors, UINT numDescriptors, UINT& heapSlot);

		void ReallocateViewHeap(UINT requiredDescriptors);
		void ReallocateSamplerHeap(UINT requiredDescriptors);

		bool CanFitView(UINT numDescriptors);
		bool CanFitSampler(UINT numDescriptors);

		UINT GetFreeViewSlots(UINT requiredDescritpors);
		UINT GetFreeSamplerSlots(UINT requiredDescritpors);
		
	private:
		Ref<DirectX12DescriptorHeap> m_CurrentViewHeap;
		Ref<DirectX12DescriptorHeap> m_CurrentSamplerHeap;
		UINT m_ViewHeapSize;
		UINT m_CurrentViewFreeSlot;
		UINT m_ViewOriginSlot;

		UINT m_SamplerHeapSize;
		UINT m_CurrentSamplerFreeSlot;
		UINT m_SamplerOriginSlot;
		DirectX12QueueType m_CmdType;
	};
};


