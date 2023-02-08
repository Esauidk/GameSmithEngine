#pragma once
#include "BindableResource.h"
#include <wrl.h>
#include <d3d12.h>
#include "d3dx12.h"
#include "DirectXMacros.h"


namespace Render {
	template <typename T>
	class VertexBuffer : public BindableResource {
	public:
		VertexBuffer() = default;

		VertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> pCommandList, T* buffer, int vertexCount) {

			int vertexBufferSize = sizeof(T) * vertexCount;
			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

			// Create the GPU and CPU buffers
			RENDER_THROW(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&vertexBufferDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&gpuBuffer)
			));

			RENDER_THROW(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&vertexBufferDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&gpuBuffer)
			));

			RENDER_THROW(pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&vertexBufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&cpuBuffer)
			));

			// Store the verte data inside
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = reinterpret_cast<BYTE*>(buffer);
			vertexData.RowPitch = vertexBufferSize;
			vertexData.SlicePitch = vertexData.RowPitch;

			UpdateSubresources(pCommandList.Get(), gpuBuffer.Get(), cpuBuffer.Get(), 0, 0, 1, &vertexData);
		}

		void Bind() override {
			return;
		}
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource2> gpuBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource2> cpuBuffer;
	};
};
