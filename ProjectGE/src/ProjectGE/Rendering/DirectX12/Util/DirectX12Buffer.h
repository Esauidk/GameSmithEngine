#pragma once
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"


namespace ProjectGE {
	template <typename T>
	class DirectX12Buffer {
	public:
		DirectX12Buffer(T* buffer, UINT count, std::string bufferName = "Personal Buffer") : m_BufferSize(sizeof(T)* count), m_State(D3D12_RESOURCE_STATE_COPY_DEST){

			auto& core = DirectX12Core::GetCore();
			auto pDevice = core.GetDevice();
			auto copyContext = core.GetCopyCommandContext();
			auto& pCommandList = copyContext->GetCommandList();

			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize);

			// Create the GPU and CPU buffers
			bool res = FAILED(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&m_GpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}", bufferName);

			auto nameConvert = std::wstring(bufferName.begin(), bufferName.end());
			m_GpuBuffer->SetName(nameConvert.c_str());

			res = FAILED(pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_CpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create CPU Buffer {0}", bufferName);
			m_CpuBuffer->SetName(nameConvert.c_str());

			// Store the data inside
			D3D12_SUBRESOURCE_DATA data = {};
			data.pData = reinterpret_cast<BYTE*>(buffer);
			data.RowPitch = m_BufferSize;
			data.SlicePitch = data.RowPitch;

			UpdateSubresources((&pCommandList), m_GpuBuffer.Get(), m_CpuBuffer.Get(), 0, 0, 1, &data);

			m_UploadSignal = copyContext->FinalizeCommandList();
		}

		DirectX12Buffer(UINT count, std::string bufferName = "Personal Buffer") : m_BufferSize(sizeof(T)* count), m_State(D3D12_RESOURCE_STATE_COPY_DEST){

			auto& core = DirectX12Core::GetCore();
			auto pDevice = core.GetDevice();

			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize);

			// Create the GPU and CPU buffers
			bool res = FAILED(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&m_GpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}", bufferName);

			auto nameConvert = std::wstring(bufferName.begin(), bufferName.end());
			m_GpuBuffer->SetName(nameConvert.c_str());

			res = FAILED(pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_CpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create CPU Buffer {0}", bufferName);
			m_CpuBuffer->SetName(nameConvert.c_str());
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUReference() {
			return m_GpuBuffer->GetGPUVirtualAddress();
		}

		void SetUploadGPUBlock() {
			auto& core = DirectX12Core::GetCore();
			core.InitializeQueueWait(DirectX12QueueType::Copy, DirectX12QueueType::Direct, m_UploadSignal);
		}

		void TransitionState(D3D12_RESOURCE_STATES newState) {
			if (newState != m_State) {
				auto& core = DirectX12Core::GetCore();
				auto& cmdList = core.GetDirectCommandContext()->GetCommandList();
				CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
					m_GpuBuffer.Get(),
					m_State, newState
				);
				cmdList->ResourceBarrier(1, &barrier);
				m_State = newState;
			}
			
		}

		UINT GetSize() const { return m_BufferSize; }
		D3D12_RESOURCE_STATES GetState() const { return m_State; }

		void UpdateData(void* newData) {
			auto& core = DirectX12Core::GetCore();
			auto copyContext = core.GetCopyCommandContext();
			auto& pCommandList = copyContext->GetCommandList();
			// Store the data inside
			D3D12_SUBRESOURCE_DATA data = {};
			data.pData = reinterpret_cast<BYTE*>(newData);
			data.RowPitch = m_BufferSize;
			data.SlicePitch = data.RowPitch;

			UpdateSubresources((&pCommandList), m_GpuBuffer.Get(), m_CpuBuffer.Get(), 0, 0, 1, &data);
			m_UploadSignal = copyContext->FinalizeCommandList();
		}

	private:
		ComPtr<ID3D12Resource2> m_GpuBuffer;
		ComPtr<ID3D12Resource2> m_CpuBuffer;
		UINT m_BufferSize;
		UINT m_UploadSignal;
		D3D12_RESOURCE_STATES m_State;
	};
};
