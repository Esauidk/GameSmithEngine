#pragma once

#include <wrl.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;

// This class is a wrapper over DX12 command lists to avoid header inclusion
class DirectX12CommandListWrapper
{
public:
	DirectX12CommandListWrapper(ComPtr<ID3D12GraphicsCommandList6> rawList);
	inline ID3D12GraphicsCommandList6* operator ->  () { return m_RawList.Get(); }
private:
	ComPtr<ID3D12GraphicsCommandList6> m_RawList;
};

