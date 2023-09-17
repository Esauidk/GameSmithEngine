#pragma once

#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Texture.h"
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12Resource.h"

namespace ProjectGE {
	
	class DirectX12TextureResource {
	public:
		DirectX12TextureResource(BYTE* data, UINT byteSize, TextureType type);
		DirectX12TextureResource(UINT byteSize, TextureType type);
	private:
		void InitializeBuffers(UINT byteSize, TextureType type);
	private:
		Ref<DirectX12Resource> m_GpuBuffer;
		Ref<DirectX12Resource> m_CpuBuffer;
		UINT m_BufferSize;
		UINT m_UploadSignal;
		bool m_Uploaded;
	};
};
