#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/Texture/DirectX12Texture.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12TextureResource.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"


namespace GameSmith {
	class DirectX12Texture2D : public Texture2D, public DirectX12Texture
	{
	public:
		friend class DirectX12RenderTexture;
		DirectX12Texture2D(char* data, UINT size);

		UINT GetWidth() const override { return m_Metadata.width; };
		UINT GetHeight() const override { return m_Metadata.height; };

		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle();
	private:
		void GenerateShaderResourceView();
	private:
		Scope<DirectX12TextureResource> m_Resource;
		TextureMetadata m_Metadata;
		DirectX12LoaderDescriptor m_TempDescriptor;
	};
};


