#pragma once
#include "gepch.h"

#include <wrl.h>
#include <d3d12.h>
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"

#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/DirectX12/CommandList/DirectCommandUtils.h"

using Microsoft::WRL::ComPtr;

namespace ProjectGE {
	enum DescriptorHeapType {
		CBVSRVUAV,
		SAMPLER,
		RT,
		ST,
		COUNT
	};
	
	D3D12_DESCRIPTOR_HEAP_TYPE ConvertHeapType(DescriptorHeapType heapType);


	class DirectX12DescriptorHeap {
	public:
		friend class DirectX12HeapDatabase;

		DirectX12DescriptorHeap(ComPtr<ID3D12DescriptorHeap> newHeap, UINT numDescriptor, DescriptorHeapType heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flags);
		inline ID3D12DescriptorHeap* GetHeapReference() { return m_CurrentHeap.Get(); }
		inline DescriptorHeapType GetType() const { return m_Type; }
		inline D3D12_DESCRIPTOR_HEAP_FLAGS GetFlags() const { return m_Flags; }

		inline UINT GetNumDescriptors() const { return m_DescriptorNum; }

		inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUReference(UINT slot) { return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_CpuStartPos, slot, m_UnitSize); }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPUReference(UINT slot) { return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_GpuStartPos, slot, m_UnitSize); }

		void AttachHeap(DirectX12QueueType cmdType);
		inline void Free() { m_Reserve = false; }

		
	private:
		inline bool IsReserved() { return m_Reserve; }
		inline void Reserve() { m_Reserve = true; }

		ComPtr<ID3D12DescriptorHeap> m_CurrentHeap;
		const UINT m_DescriptorNum;
		const DescriptorHeapType m_Type;
		const D3D12_DESCRIPTOR_HEAP_FLAGS m_Flags;
		const UINT m_UnitSize;
		const D3D12_CPU_DESCRIPTOR_HANDLE m_CpuStartPos;
		const D3D12_GPU_DESCRIPTOR_HANDLE m_GpuStartPos;

		bool m_Reserve;
	};

	class DirectX12HeapDatabase
	{
	public:
		DirectX12HeapDatabase() = default;
		// Reuse or allocate new resource space on GPU heap
		Ref<DirectX12DescriptorHeap> AllocateHeap(UINT numDescriptor, DescriptorHeapType heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flags);
	private:
		std::vector<Ref<DirectX12DescriptorHeap>> m_AvailableHeaps;
	};
};


