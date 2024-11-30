#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/Texture/DirectX12Texture.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12TextureResource.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"

namespace GameSmith {
	enum class RTState {
		READ,
		WRITE,
		COPYSRC,
		COPYDST
	};

	class DirectX12RenderTexture : public RenderTexture, public DirectX12Texture {
	public:
		DirectX12RenderTexture(unsigned int width, unsigned int height);
		DirectX12RenderTexture(unsigned int width, unsigned int height, float* clearColor);
		void ChangeState(RTState newState, bool force = false);

		inline D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetHandle() { return m_RTDescriptor; }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return m_SRVDescriptor; }

		void CopyToResource(Ref<DirectX12Resource> otherResource, DirectX12CommandContextBase* context);
		void CopyToResource(Ref<DirectX12RenderTexture> otherTex, DirectX12CommandContextBase* context);

		void UpdateSize(unsigned int width, unsigned height);

		UINT GetWidth() const override { return m_Metadata.width; };
		UINT GetHeight() const override { return m_Metadata.height; };
		void ClearTexture() override;
		void CopyTexture(Ref<Texture2D> tex) override;
		void CopyTexture(Ref<RenderTexture> tex) override;

		const float* GetClearColor() const override { return m_Metadata.clearColor; }
		inline bool WindowResized(WindowResizeEvent& e) { UpdateSize(e.GetWidth(), e.GetHeight()); return false; };
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
