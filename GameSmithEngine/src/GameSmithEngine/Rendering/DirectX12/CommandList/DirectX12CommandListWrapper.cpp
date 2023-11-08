#include "gepch.h"
#include "DirectX12CommandListWrapper.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	DirectX12CommandListWrapper::DirectX12CommandListWrapper(ComPtr<ID3D12GraphicsCommandList6> rawList) : m_RawList(rawList), m_IsOpen(true)
	{
	}

	void DirectX12CommandListWrapper::CloseList()
	{
		GE_CORE_ASSERT(m_IsOpen, "Command List Has Already Been Closed");
		
		m_RawList->Close();
		m_IsOpen = false;
	}
};

