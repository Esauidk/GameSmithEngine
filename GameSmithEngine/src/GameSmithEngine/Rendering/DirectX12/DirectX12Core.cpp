#include "gepch.h"
#include "DirectX12Core.h"

#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/BufferLayout.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

namespace GameSmith {
	Scope<DirectX12Core> DirectX12Core::m_Core = nullptr;

	DirectX12Core& DirectX12Core::CreateCore()
	{
		GE_CORE_ASSERT(m_Core == nullptr, "There already exists an instance of the DirectX12Core");

		m_Core = Scope<DirectX12Core>(new DirectX12Core());

		m_Core->Init();

		return GetCore();
	}

	DirectX12Core::DirectX12Core() {
		bool res;

		UINT createFactoryFlags = 0;

#if defined(GE_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		res = FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&m_Factory)));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 DXGI factory");


		// Create the Debug Layer (Allows debuging on the device)
#if defined(GE_DEBUG)
		res = FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Debug)));

		GE_CORE_ASSERT(!res, "Failed to create a DirectX12 debug interface");

		m_Debug->EnableDebugLayer();

#endif
		// Create Device
		res = FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device)));

		GE_CORE_ASSERT(!res, "Failed to create DirectX12 device");



#if defined(GE_DEBUG)
		// Create Info Queue, shows debug logs depending on severity and sets breaks
		res = FAILED(m_Device.As(&m_InfoQueue));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 info queue");

		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);


		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);


		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

#endif
	}

	void DirectX12Core::Init()
	{
		m_HeapDB = Scope<DirectX12HeapDatabase>(new DirectX12HeapDatabase());

		for (UINT heapType = DescriptorHeapType::CBVSRVUAV; heapType < DescriptorHeapType::COUNT; heapType++) {
			m_DescriptorLoaders.emplace_back((DescriptorHeapType)heapType);
		}

		m_DirectContext = Scope<DirectX12CommandContextDirect>(new DirectX12CommandContextDirect());
		m_CopyContext = Scope<DirectX12CommandContextCopy>(new DirectX12CommandContextCopy());;
		m_DirectContext->FinalizeCommandList();
		m_CopyContext->FinalizeCommandList();

		m_Defaults.EmptyCBV.m_View = m_DescriptorLoaders[DescriptorHeapType::CBVSRVUAV].AllocateSlot();
		m_Defaults.EmptySRV.m_View = m_DescriptorLoaders[DescriptorHeapType::CBVSRVUAV].AllocateSlot();
		m_Defaults.EmptySampler.m_View = m_DescriptorLoaders[DescriptorHeapType::SAMPLER].AllocateSlot();
	}

	void DirectX12Core::Destroy()
	{
		InitializeCPUQueueWait(DirectX12QueueType::Direct);
	}

	DirectX12Core& DirectX12Core::GetCore() {
		GE_CORE_ASSERT(m_Core != nullptr, "There is no Core instance available");
		return *(m_Core.get());
	}

	DirectX12CommandQueue& DirectX12Core::FindQueue(DirectX12QueueType type)
	{
		switch (type) {
		case DirectX12QueueType::Direct:
			return m_DirectContext->GetQueue();
		case DirectX12QueueType::Copy:
			return m_CopyContext->GetQueue();
		}
		return m_DirectContext->GetQueue();
	}

	
	void DirectX12Core::InitializeQueueWait(DirectX12QueueType executor, DirectX12QueueType waiter, UINT fenceVal)
	{
		auto& execQueue = FindQueue(executor);
		auto& waiterQueue = FindQueue(waiter);
		waiterQueue.GPUWaitForFenceValue(execQueue, fenceVal);
	}

	void DirectX12Core::InitializeCPUQueueWait(DirectX12QueueType target)
	{
		auto& targetQueue = FindQueue(target);
		targetQueue.Flush();
	}

	void DirectX12Core::InitializeCPUQueueWait(UINT fenceVal, DirectX12QueueType target)
	{
		auto& targetQueue = FindQueue(target);
		targetQueue.CPUWaitForFenceValue(fenceVal);
	}
}