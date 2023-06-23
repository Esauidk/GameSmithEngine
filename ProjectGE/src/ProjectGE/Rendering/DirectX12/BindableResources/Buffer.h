#pragma once
#include "BindableResource.h"
#include "ProjectGE/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"


namespace ProjectGE {
	template <typename T>
	class Buffer : public BindableResource {
	public:
		~Buffer() = default;

		Buffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> pCommandList, T* buffer, UINT count, std::string bufferName = "Personal Buffer") {

			bufferSize = sizeof(T) * count;
			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

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

			std::wstring nameConvert = std::wstring(bufferName.begin(), bufferName.end());
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
			data.RowPitch = bufferSize;
			data.SlicePitch = data.RowPitch;

			UpdateSubresources(pCommandList.Get(), m_GpuBuffer.Get(), m_CpuBuffer.Get(), 0, 0, 1, &data);
		}

		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override {
			return;
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D12Resource2> m_GpuBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource2> m_CpuBuffer;
		UINT bufferSize;
	};
};
