#include "gepch.h"
#include "DirectX12TextureResource.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"

namespace ProjectGE {
	DirectX12TextureResource::DirectX12TextureResource(BYTE* data, UINT byteSize, TextureType type)
	{
		InitializeBuffers(byteSize, type);
	}
	DirectX12TextureResource::DirectX12TextureResource(UINT byteSize, TextureType type)
	{
		InitializeBuffers(byteSize, type);
	}

	void DirectX12TextureResource::InitializeBuffers(UINT byteSize, TextureType type)
	{
		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferDesc;
		switch (type) {
		case TextureType::Tex2D:
			break;
			// TODO: Finish texture resource description
			//DXGI_FORMAT_
			//bufferDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_BufferSize);
		}
	}
};

