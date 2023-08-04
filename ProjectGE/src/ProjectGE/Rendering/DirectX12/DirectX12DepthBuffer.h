#pragma once
#include <wrl.h>
#include <d3d12.h>

namespace ProjectGE {
	class DirectX12DepthBuffer
	{
	public:
		~DirectX12DepthBuffer() = default;
		DirectX12DepthBuffer(ID3D12Device8* pDevice, UINT initialW, UINT initialH);
		void Clear(ID3D12GraphicsCommandList6* cmdList, float newDepth = 1.0f);
		void Resize(ID3D12Device8* pDevice, UINT newW, UINT newH);
		D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() const;
	private:
		// Depth Stencil Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DSVHeap;

		Microsoft::WRL::ComPtr<ID3D12Resource2> m_DepthBuffer;
	};
};


