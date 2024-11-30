#include "gepch.h"
#include "DirectX12Texture2D.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"

#include "stb_image.h"

namespace GameSmith {
	DirectX12Texture2D::DirectX12Texture2D(char* data, UINT size)
	{
		int width, height, channels;
		stbi_uc* image = stbi_load_from_memory((stbi_uc*)data, size, &width, &height, &channels, 0);
		GE_CORE_ASSERT(image != nullptr, "Failed to load image");

		m_Metadata.width = width;
		m_Metadata.height = height;
		m_Metadata.channels = channels;
		m_Metadata.mips = 1;
		m_Resource = Scope<DirectX12TextureResource>(new DirectX12TextureResource((BYTE*)image, m_Metadata, TextureType::Tex2D));

		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();

		GenerateShaderResourceView();
		stbi_image_free(image);
	}

	void DirectX12Texture2D::GenerateShaderResourceView()
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Texture2D.MipLevels = m_Metadata.mips;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.ResourceMinLODClamp = 0.0f;
		desc.Texture2D.PlaneSlice = 0;

		DirectX12Core::GetCore().GetDevice()->CreateShaderResourceView(m_Resource->GetResource(), &desc, m_TempDescriptor);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Texture2D::GetSRVHandle()
	{
		auto& core = DirectX12Core::GetCore();
		if (m_Resource->GetStateTracker().GetState() == D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Resource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, core.GetDirectCommandContext());
			core.GetDirectCommandContext()->FinalizeResourceBarriers();
		}

		return m_TempDescriptor;
	}
};

