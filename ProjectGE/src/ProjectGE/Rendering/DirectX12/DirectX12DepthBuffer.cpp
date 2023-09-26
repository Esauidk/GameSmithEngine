#include "gepch.h"
#include "DirectX12DepthBuffer.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

namespace ProjectGE {
	DirectX12DepthBuffer::DirectX12DepthBuffer(ID3D12Device8* pDevice, UINT initialW, UINT initialH) {
        m_Descriptor = DirectX12Core::GetCore().GetDescriptorLoader(DescriptorHeapType::ST).AllocateSlot();

        D3D12_CLEAR_VALUE optimizedClearValue = {};
        optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
        optimizedClearValue.DepthStencil = { 1.0f, 0 };

        CD3DX12_HEAP_PROPERTIES properties(D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, initialW, initialH,
            1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
        bool res = FAILED(pDevice->CreateCommittedResource(
            &properties,
            D3D12_HEAP_FLAG_NONE,
            &resdesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &optimizedClearValue,
            IID_PPV_ARGS(&m_DepthBuffer)
        ));

        GE_CORE_ASSERT(!res, "Failed to create depth buffer");

        D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
        dsv.Format = DXGI_FORMAT_D32_FLOAT;
        dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsv.Texture2D.MipSlice = 0;
        dsv.Flags = D3D12_DSV_FLAG_NONE;

        pDevice->CreateDepthStencilView(m_DepthBuffer.Get(), &dsv, m_Descriptor);
	}

    void DirectX12DepthBuffer::Clear(ID3D12GraphicsCommandList6* cmdList, float newDepth) {
        cmdList->ClearDepthStencilView(m_Descriptor, D3D12_CLEAR_FLAG_DEPTH, newDepth, 0, 0, nullptr);
    }

    void DirectX12DepthBuffer::Resize(ID3D12Device8* pDevice, UINT newW, UINT newH)
    {
        D3D12_CLEAR_VALUE optimizedClearValue = {};
        optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
        optimizedClearValue.DepthStencil = { 1.0f, 0 };

        CD3DX12_HEAP_PROPERTIES properties(D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, newW, newH,
            1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
        bool res = FAILED(pDevice->CreateCommittedResource(
            &properties,
            D3D12_HEAP_FLAG_NONE,
            &resdesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &optimizedClearValue,
            IID_PPV_ARGS(&m_DepthBuffer)
        ));

        GE_CORE_ASSERT(!res, "Failed to create depth buffer");

        D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
        dsv.Format = DXGI_FORMAT_D32_FLOAT;
        dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsv.Texture2D.MipSlice = 0;
        dsv.Flags = D3D12_DSV_FLAG_NONE;

        pDevice->CreateDepthStencilView(m_DepthBuffer.Get(), &dsv, m_Descriptor);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DirectX12DepthBuffer::GetHandle() const {
        return m_Descriptor;
    }
};
