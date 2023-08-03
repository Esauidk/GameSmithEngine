#pragma once
#include "gepch.h"
#include "ProjectGE/Rendering/DirectX12/CommandList/DirectX12CommandListWrapper.h"

// This class manages the lifetime and usage of command lists for DX12 in this engine
class DirectX12CommandContext
{
public:
	void StartCommandList();
	void CloseCommandList();
	inline DirectX12CommandListWrapper* GetCommandList() { return m_CurrentList.get(); };
private:
	std::unique_ptr<DirectX12CommandListWrapper> m_CurrentList;
	std::queue<UINT> m_SyncPoints;

};

