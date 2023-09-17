#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Texture.h"
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12Buffer.h"
#include "ProjectGE/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"

namespace ProjectGE {
	class DirectX12Texture2D : public Texture2D
	{
	public :
		DirectX12Texture2D(const std::string& path);

		UINT GetWidth() const override { return m_Widght; };
		UINT GetHeight() const override { return m_Height; };

		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor();
	private:
		Scope<DirectX12Buffer<BYTE>> m_Buffer;
		std::string m_Path;
		UINT m_Widght;
		UINT m_Height;
		DirectX12LoaderDescriptor m_TempDescriptor;
	};
};


