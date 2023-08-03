#include "gepch.h"
#include "DirectX12CommandListWrapper.h"

DirectX12CommandListWrapper::DirectX12CommandListWrapper(ComPtr<ID3D12GraphicsCommandList6> rawList) : m_RawList(rawList) 
{
}
