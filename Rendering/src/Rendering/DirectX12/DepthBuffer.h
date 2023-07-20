#pragma once
#include <wrl.h>
#include <d3d12.h>

namespace Render {
	class DepthBuffer
	{
	public:
		~DepthBuffer() = default;
		DepthBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, UINT initialW, UINT initialH);
		void Clear(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList, float newDepth = 1.0f);
		D3D12_CPU_DESCRIPTOR_HANDLE GetHandle();
	private:
		// Depth Stencil Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DSVHeap;

		Microsoft::WRL::ComPtr<ID3D12Resource2> m_DepthBuffer;
	};
};


