#include "gepch.h"
#include "DirectX12TextureResource.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	DirectX12TextureResource::DirectX12TextureResource(BYTE* data, TextureMetadata metadata, TextureType type) : m_Metadata(metadata)
	{
		InitializeBuffers(metadata, type);
		UpdateData(data);
	}
	DirectX12TextureResource::DirectX12TextureResource(TextureMetadata metadata, TextureType type) : m_Metadata(metadata)
	{
		InitializeBuffers(metadata, type);
	}

	void DirectX12TextureResource::UpdateData(BYTE* data)
	{
		auto& core = DirectX12Core::GetCore();
		auto& copyContext = core.GetCopyCommandContext();
		auto& pCommandList = copyContext.GetCommandList();

		D3D12_SUBRESOURCE_DATA dataDesc = {};
		dataDesc.pData = reinterpret_cast<BYTE*>(data);
		dataDesc.RowPitch = m_Metadata.width * m_Metadata.channels;
		dataDesc.SlicePitch = m_Metadata.width * m_Metadata.height * m_Metadata.channels;

		UpdateSubresources((&pCommandList), m_GpuBuffer->GetResource(), m_CpuBuffer->GetResource(), 0, 0, 1, &dataDesc);

		m_UploadSignal = copyContext.FinalizeCommandList();
		m_Uploaded = true;
	}

	void DirectX12TextureResource::SetUploadGPUBlock()
	{
		if (m_Uploaded) {
			auto& core = DirectX12Core::GetCore();
			core.InitializeQueueWait(DirectX12QueueType::Copy, DirectX12QueueType::Direct, m_UploadSignal);
			m_Uploaded = false;
		}
	}

	void DirectX12TextureResource::InitializeBuffers(TextureMetadata metadata, TextureType type)
	{
		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC textureDesc;
		switch (type) {
		case TextureType::Tex2D:
			
			// TODO: Finish texture resource description
			textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				metadata.width, 
				metadata.height,
				1
			);
			break;
		}

		auto& core = DirectX12Core::GetCore();
		auto pDevice = core.GetDevice();

		ComPtr<ID3D12Resource2> cpuBuffer;
		ComPtr<ID3D12Resource2> gpuBuffer;
		// Create the GPU and CPU buffers
		bool res = FAILED(pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&gpuBuffer)
		));

		GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}");

		std::wstring nameConvert = L"Texture Resource";
		gpuBuffer->SetName(nameConvert.c_str());

		UINT64 interSize = GetRequiredIntermediateSize(gpuBuffer.Get(), 0, 1) + D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(interSize);
		res = FAILED(pDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&cpuBuffer)
		));


		GE_CORE_ASSERT(!res, "Failed to create CPU Buffer {0}");
		cpuBuffer->SetName(nameConvert.c_str());

		m_GpuBuffer = std::make_shared<DirectX12Resource>(gpuBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST);
		m_CpuBuffer = std::make_shared<DirectX12Resource>(cpuBuffer.Get(), D3D12_RESOURCE_STATE_GENERIC_READ);
	}
};

