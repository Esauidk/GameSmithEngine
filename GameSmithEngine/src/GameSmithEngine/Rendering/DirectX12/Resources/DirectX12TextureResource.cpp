#include "gepch.h"
#include "DirectX12TextureResource.h"
#include "GameSmithEngine/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	DirectX12TextureResource::DirectX12TextureResource(BYTE* data, TextureMetadata metadata, TextureType type, TextureMisc extra) : m_Metadata(metadata)
	{
		InitializeBuffers(metadata, type, extra);
		UpdateData(data);
		SetUploadGPUBlock();
	}

	DirectX12TextureResource::DirectX12TextureResource(TextureMetadata metadata, TextureType type, TextureMisc extra) : m_Metadata(metadata)
	{
		InitializeBuffers(metadata, type, extra);
	}

	void DirectX12TextureResource::UpdateData(BYTE* data)
	{
		auto& core = DirectX12Core::GetCore();
		auto copyContext = core.GetCopyCommandContext();
		auto& pCommandList = copyContext->GetCommandList();

		auto directContext = core.GetDirectCommandContext();
		m_GpuBuffer->GetResourceStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST, directContext);
		directContext->FinalizeResourceBarriers();

		D3D12_SUBRESOURCE_DATA dataDesc = {};
		dataDesc.pData = reinterpret_cast<BYTE*>(data);
		dataDesc.RowPitch = m_Metadata.width * m_Metadata.channels;
		dataDesc.SlicePitch = m_Metadata.width * m_Metadata.height * m_Metadata.channels;

		UpdateSubresources((&pCommandList), m_GpuBuffer->GetResource(), m_CpuBuffer->GetResource(), 0, 0, 1, &dataDesc);

		copyContext->FinalizeCommandList();
		m_GpuBuffer->GetResourceStateTracker().UndoTransition(directContext);
		m_Uploaded = true;
	}

	void DirectX12TextureResource::SetUploadGPUBlock()
	{
		if (m_Uploaded) {
			auto& core = DirectX12Core::GetCore();
			core.InitializeQueueWait(DirectX12QueueType::Direct, DirectX12QueueType::Copy, core.GetDirectCommandContext()->GetLastSubmissionID());
			core.GetCopyCommandContext()->RequestWait(DirectX12QueueType::Direct);
			m_Uploaded = false;
		}
	}

	void DirectX12TextureResource::InitializeBuffers(TextureMetadata metadata, TextureType type, TextureMisc extra)
	{
		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC textureDesc;
		CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM, metadata.clearColor);
		D3D12_RESOURCE_FLAGS texFlags;

		switch (extra) {
		case TextureMisc::RT:
			texFlags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			break;
		case TextureMisc::DT:
			texFlags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			break;
		default:
			texFlags = D3D12_RESOURCE_FLAG_NONE;
		}


		switch (type) {

		case TextureType::Tex2D:
			
			// TODO: Finish texture resource description
			textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				metadata.width, 
				metadata.height,
				1,
				metadata.mips,
				1,
				0,
				texFlags,
				D3D12_TEXTURE_LAYOUT_UNKNOWN,
				0
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
			extra == TextureMisc::Normal ? NULL : &clearValue,
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

		m_GpuBuffer = Ref<DirectX12Resource>(new DirectX12Resource(gpuBuffer.Get(), D3D12_RESOURCE_STATE_COMMON));
		m_CpuBuffer = Ref<DirectX12Resource>(new DirectX12Resource(cpuBuffer.Get(), D3D12_RESOURCE_STATE_GENERIC_READ));
	}
};

