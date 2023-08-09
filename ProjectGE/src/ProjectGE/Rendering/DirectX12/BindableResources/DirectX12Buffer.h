#pragma once
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"


namespace ProjectGE {
	template <typename T>
	class DirectX12Buffer {
	protected:

		DirectX12Buffer(T* buffer, UINT count, std::string bufferName = "Personal Buffer") : m_BufferSize(sizeof(T)* count) {

			auto pDevice = DirectX12Context::GetDevice();
			auto& pCommandList = DirectX12Context::GetCopyCommandList();

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

			m_UploadSignal = DirectX12Context::FinalizeCommandList(DirectX12QueueType::Copy);
		}

	protected:
		ComPtr<ID3D12Resource2> m_GpuBuffer;
		ComPtr<ID3D12Resource2> m_CpuBuffer;
		UINT m_BufferSize;
		UINT m_UploadSignal;
	};
};
