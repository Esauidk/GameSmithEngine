#include "gepch.h"
#include "DirectX12Texture.h"

#include "stb_image.h"

#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Core/Application.h"

namespace ProjectGE {
	DirectX12Texture2D::DirectX12Texture2D(const std::string& path) : m_Path(path)
	{
		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		GE_CORE_ASSERT(data, "Failed to load image");

		m_Buffer = std::make_unique<DirectX12Buffer<BYTE>>(data, width * height * channels, "Texture2D");

		stbi_image_free(data);
	}

	void DirectX12Texture2D::Bind()
	{
		//m_Buffer->TransitionState(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		
	}
};

