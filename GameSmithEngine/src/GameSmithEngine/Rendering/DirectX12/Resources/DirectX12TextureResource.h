#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12Resource.h"

namespace GameSmith {
	
	class DirectX12TextureResource {
	public:
		DirectX12TextureResource(BYTE* data, TextureMetadata metadata, TextureType type);
		DirectX12TextureResource(TextureMetadata metadata, TextureType type);
		void UpdateData(BYTE* data);

		void SetUploadGPUBlock();
		DirectX12BarrierTracker& GetStateTracker() { return m_GpuBuffer->GetResourceStateTracker(); }

		ID3D12Resource* GetResource() { return m_GpuBuffer->GetResource(); }
	private:
		void InitializeBuffers(TextureMetadata metadata, TextureType type);
	private:
		TextureMetadata m_Metadata;

		Ref<DirectX12Resource> m_GpuBuffer;
		Ref<DirectX12Resource> m_CpuBuffer;
		UINT m_BufferSize;
		UINT m_UploadSignal;
		bool m_Uploaded;
	};
};
