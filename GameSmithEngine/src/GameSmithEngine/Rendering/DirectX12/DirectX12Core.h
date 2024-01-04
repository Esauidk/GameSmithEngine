#pragma once
#include "GameSmithEngine/Rendering/DirectX12/CommandList/DirectX12CommandContext.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12HeapDatabase.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>


using Microsoft::WRL::ComPtr;

namespace GameSmith {

	/**
	* 
	**/
	class DirectX12Core {
	public:
		static DirectX12Core& CreateCore();
		static DirectX12Core& GetCore();
		inline ID3D12Device8* GetDevice() { return m_Device.Get(); }
		inline IDXGIFactory5* GetFactory() { return m_Factory.Get(); }
		inline void FrameCompletedRecording() { m_FrameIds.push(m_DirectContext->GetLastSubmissionID()); }
		inline void FrameSkipped() { if (!m_FrameIds.empty()) { m_FrameIds.pop(); GE_CORE_INFO("Skipping this frame"); } }
		inline void SwappingFrame() { 
			if (!m_FrameIds.empty()) { 
				InitializeCPUQueueWait(m_FrameIds.front(), Direct); 
				m_FrameIds.pop(); 
			} 
		}
		inline bool FrameReady() { return m_FrameIds.empty() || FindQueue(Direct).IsFenceComplete(m_FrameIds.front()); }
		inline DirectX12CommandContextDirect& GetDirectCommandContext() { return *m_DirectContext; }
		inline DirectX12CommandContextCopy& GetCopyCommandContext() { return *m_CopyContext; }
		inline DirectX12HeapDatabase& GetHeapDatabase() { return *m_HeapDB; }
		inline DirectX12DefaultViews& GetDefaultViews() { return m_Defaults; }

		inline DirectX12DescriptorLoaderManager& GetDescriptorLoader(DescriptorHeapType type) { return m_DescriptorLoaders[(int)type]; }
		// Tell one queue to wait for another queue to complete a certain amount of work
		void InitializeQueueWait(DirectX12QueueType executor, DirectX12QueueType waiter, UINT fenceVal);
		// Tell the CPU to wait for all work to complete on a specific queue
		void InitializeCPUQueueWait(DirectX12QueueType target);
		// Tell the CPU to wait for some work to be complete on a specific queue
		void InitializeCPUQueueWait(UINT fenceVal, DirectX12QueueType target);
	private:
		DirectX12Core();
		void Init();
		DirectX12CommandQueue& FindQueue(DirectX12QueueType type);
	private:
		ComPtr<ID3D12Debug> m_Debug;
		ComPtr<ID3D12InfoQueue> m_InfoQueue;
		ComPtr<ID3D12Device8> m_Device;
		ComPtr<IDXGIFactory5> m_Factory;
		Scope<DirectX12CommandContextDirect> m_DirectContext;
		Scope<DirectX12CommandContextCopy> m_CopyContext;
		Scope<DirectX12HeapDatabase> m_HeapDB;
		static Scope<DirectX12Core> m_Core;

		std::vector<DirectX12DescriptorLoaderManager> m_DescriptorLoaders;

		DirectX12DefaultViews m_Defaults;

		std::queue<unsigned int> m_FrameIds;
	};
};
