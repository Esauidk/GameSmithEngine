#include "gepch.h"
#include "DirectX12Texture2D.h"

#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

namespace ProjectGE {
	DirectX12Texture2D::DirectX12Texture2D(const std::string& path) : m_Path(path)
	{
		int width, height, channels;
		m_Image = stbi_load(path.c_str(), &width, &height, &channels, 0);
		GE_CORE_ASSERT(m_Image != nullptr, "Failed to load image");

		m_Metadata.width = width;
		m_Metadata.height = height;
		m_Metadata.channels = channels;
		m_Metadata.mips = 1;
		m_Resource = std::make_unique<DirectX12TextureResource>((BYTE*)m_Image, m_Metadata, TextureType::Tex2D);

		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();

		GenerateShaderResourceView();
	}

	DirectX12Texture2D::~DirectX12Texture2D()
	{
		stbi_image_free(m_Image);
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

	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Texture2D::GetDescriptor()
	{
		if (m_Resource->GetStateTracker().GetState() == D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Resource->SetUploadGPUBlock();
			m_Resource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}

		return m_TempDescriptor;
	}

	void DirectX12Texture2D::Test()
	{
		D3D12_RESOURCE_STATES originalState = m_Resource->GetStateTracker().GetState();
		if (originalState != D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Resource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST);
		}

		m_Resource->UpdateData((BYTE*)m_Image);

		if (m_Resource->GetStateTracker().GetState() != originalState) {
			m_Resource->SetUploadGPUBlock();
			m_Resource->GetStateTracker().TransitionBarrier(originalState);
		}
	}


};

