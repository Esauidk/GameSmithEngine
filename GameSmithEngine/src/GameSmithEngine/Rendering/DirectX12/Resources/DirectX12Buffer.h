#pragma once
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12Resource.h"

#define MIN_BUFF_SIZE 256

namespace GameSmith {
	template <typename T>
	class DirectX12Buffer {
	public:
		DirectX12Buffer(T* buffer, UINT count, std::string bufferName = "Personal Buffer") : m_BufferSize(sizeof(T)* count), m_Uploaded(true){

			auto& core = DirectX12Core::GetCore();
			auto pDevice = core.GetDevice();
			auto copyContext = core.GetCopyCommandContext();
			auto& pCommandList = copyContext->GetCommandList();

			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize);

			ComPtr<ID3D12Resource2> cpuBuffer;
			ComPtr<ID3D12Resource2> gpuBuffer;
			// Create the GPU and CPU buffers
			bool res = FAILED(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&gpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}", bufferName);

			auto nameConvert = std::wstring(bufferName.begin(), bufferName.end());
			gpuBuffer->SetName(nameConvert.c_str());

			res = FAILED(pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&cpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create CPU Buffer {0}", bufferName);
			cpuBuffer->SetName(nameConvert.c_str());

			// Store the data inside
			D3D12_SUBRESOURCE_DATA data = {};
			data.pData = reinterpret_cast<BYTE*>(buffer);
			data.RowPitch = m_BufferSize;
			data.SlicePitch = data.RowPitch;

			UpdateSubresources((&pCommandList), gpuBuffer.Get(), cpuBuffer.Get(), 0, 0, 1, &data);

			copyContext->FinalizeCommandList();

			m_GpuBuffer = Ref<DirectX12Resource>(new DirectX12Resource(gpuBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST));
			m_CpuBuffer = Ref<DirectX12Resource>(new DirectX12Resource(cpuBuffer.Get(), D3D12_RESOURCE_STATE_GENERIC_READ));
			m_Uploaded = true;

			SetUploadGPUBlock();
		}

		DirectX12Buffer(UINT count, std::string bufferName = "Personal Buffer") : m_BufferSize(sizeof(T)* count), m_Uploaded(false) {

			auto& core = DirectX12Core::GetCore();
			auto pDevice = core.GetDevice();

			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize);

			ComPtr<ID3D12Resource2> cpuBuffer;
			ComPtr<ID3D12Resource2> gpuBuffer;

			// Create the GPU and CPU buffers
			bool res = FAILED(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&gpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}", bufferName);

			auto nameConvert = std::wstring(bufferName.begin(), bufferName.end());
			gpuBuffer->SetName(nameConvert.c_str());

			res = FAILED(pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&cpuBuffer)
			));


			GE_CORE_ASSERT(!res, "Failed to create CPU Buffer {0}", bufferName);
			cpuBuffer->SetName(L"Upload Buffer");

			m_GpuBuffer = Ref<DirectX12Resource>(new DirectX12Resource(gpuBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST));
			m_CpuBuffer = Ref<DirectX12Resource>(new DirectX12Resource(cpuBuffer.Get(), D3D12_RESOURCE_STATE_GENERIC_READ));

			if (m_BufferSize < MIN_BUFF_SIZE) {
				m_BufferSize = MIN_BUFF_SIZE;
			}
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUReference() {
			return m_GpuBuffer->GetResource()->GetGPUVirtualAddress();
		}

		void SetUploadGPUBlock() {
			if (m_Uploaded) {
				auto& core = DirectX12Core::GetCore();
				core.InitializeQueueWait(DirectX12QueueType::Direct, DirectX12QueueType::Copy, core.GetDirectCommandContext()->GetLastSubmissionID());
				core.GetCopyCommandContext()->RequestWait(DirectX12QueueType::Direct);
				m_Uploaded = false;
			}
			
		}

		DirectX12BarrierTracker& GetStateTracker() {
			return m_GpuBuffer->GetResourceStateTracker();
		}

		UINT GetSize() const { return m_BufferSize; }

		void UpdateData(void* newData, UINT byteSize) {
			GE_CORE_ASSERT(byteSize <= m_BufferSize, "Data passed in won't fit in buffer");

			auto& core = DirectX12Core::GetCore();
			auto copyContext = core.GetCopyCommandContext();
			auto& pCommandList = copyContext->GetCommandList();
			// Store the data inside
			D3D12_SUBRESOURCE_DATA data = {};
			data.pData = reinterpret_cast<BYTE*>(newData);
			data.RowPitch = byteSize;
			data.SlicePitch = data.RowPitch;

			UpdateSubresources((&pCommandList), m_GpuBuffer->GetResource(), m_CpuBuffer->GetResource(), 0, 0, 1, &data);
			copyContext->FinalizeCommandList();
			m_Uploaded = true;
		}

		DirectX12Resource* GetResource() { return m_GpuBuffer.get(); }
	private:
		Ref<DirectX12Resource> m_GpuBuffer;
		Ref<DirectX12Resource> m_CpuBuffer;
		UINT m_BufferSize;
		bool m_Uploaded;
	};
};
