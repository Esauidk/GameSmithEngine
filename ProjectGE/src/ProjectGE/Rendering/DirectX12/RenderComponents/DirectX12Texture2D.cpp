#include "gepch.h"
#include "DirectX12Texture2D.h"

#include "stb_image.h"

#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

namespace ProjectGE {
	DirectX12Texture2D::DirectX12Texture2D(const std::string& path) : m_Path(path)
	{
		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		GE_CORE_ASSERT(data != nullptr, "Failed to load image");

		m_Metadata.width = width;
		m_Metadata.height = height;
		m_Metadata.channels = channels;
		m_Resource = std::make_unique<DirectX12TextureResource>((BYTE*)data, m_Metadata, TextureType::Tex2D);

		stbi_image_free(data);

		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Texture2D::GetDescriptor()
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Texture2D.MipLevels = 0;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.ResourceMinLODClamp = 0.0f;
		desc.Texture2D.PlaneSlice = 0;

		DirectX12Core::GetCore().GetDevice()->CreateShaderResourceView(m_Resource->GetResource(), &desc, m_TempDescriptor);

		return m_TempDescriptor;
	}


};

