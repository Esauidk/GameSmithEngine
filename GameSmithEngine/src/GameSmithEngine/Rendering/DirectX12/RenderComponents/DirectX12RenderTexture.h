#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12TextureResource.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"

namespace GameSmith {
	enum class RTState {
		READ,
		WRITE,
		COPY
	};

	class DirectX12RenderTexture : public RenderTexture {
	public:
		DirectX12RenderTexture(unsigned int width, unsigned int height);
		void ChangeState(RTState newState, bool force = false);

		inline D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetHandle() { return m_RTDescriptor; }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return m_SRVDescriptor; }


		void CopyToResource(Ref<DirectX12Resource> otherResource, DirectX12CommandContextBase* context);
		void UpdateSize(unsigned int width, unsigned height);
		UINT GetWidth() const override { return m_Metadata.width; };
		UINT GetHeight() const override { return m_Metadata.height; };
		const float* GetClearColor() const override { return m_Metadata.clearColor; }
	private:
		void GenerateViews();
	private:
		Scope<DirectX12TextureResource> m_TextureResource;
		TextureMetadata m_Metadata;
		DirectX12LoaderDescriptor m_RTDescriptor;
		DirectX12LoaderDescriptor m_SRVDescriptor;
		RTState m_State;
		RTState m_PrevState;
	};
};
