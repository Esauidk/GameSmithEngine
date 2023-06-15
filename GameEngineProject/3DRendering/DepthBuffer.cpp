#include "DepthBuffer.h"
#include "d3dx12.h"
#include "DirectXMacros.h"

namespace Render {
	DepthBuffer::DepthBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, UINT initialW, UINT initialH) {

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		RENDER_THROW(pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&pDSVHeap)));

        D3D12_CLEAR_VALUE optimizedClearValue = {};
        optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
        optimizedClearValue.DepthStencil = { 1.0f, 0 };

        CD3DX12_HEAP_PROPERTIES properties(D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, initialW, initialH,
            1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
        RENDER_THROW(pDevice->CreateCommittedResource(
            &properties,
            D3D12_HEAP_FLAG_NONE,
            &resdesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &optimizedClearValue,
            IID_PPV_ARGS(&pDepthBuffer)
        ));

        D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
        dsv.Format = DXGI_FORMAT_D32_FLOAT;
        dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsv.Texture2D.MipSlice = 0;
        dsv.Flags = D3D12_DSV_FLAG_NONE;

        pDevice->CreateDepthStencilView(pDepthBuffer.Get(), &dsv,
            pDSVHeap->GetCPUDescriptorHandleForHeapStart());
	}

    void DepthBuffer::Clear(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList, float newDepth) {
        cmdList->ClearDepthStencilView(pDSVHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, newDepth, 0, 0, nullptr);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DepthBuffer::GetHandle() {
        return pDSVHeap->GetCPUDescriptorHandleForHeapStart();
    }
};
