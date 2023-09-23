#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12TextureResource.h"
#include "ProjectGE/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"

#include "stb_image.h"

namespace ProjectGE {
	class DirectX12Texture2D : public Texture2D
	{
	public :
		DirectX12Texture2D(const std::string& path);
		~DirectX12Texture2D();

		UINT GetWidth() const override { return m_Metadata.width; };
		UINT GetHeight() const override { return m_Metadata.height; };

		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor();
	private:
		Scope<DirectX12TextureResource> m_Resource;
		stbi_uc* m_Image;
		std::string m_Path;
		TextureMetadata m_Metadata;
		DirectX12LoaderDescriptor m_TempDescriptor;
	};
};


