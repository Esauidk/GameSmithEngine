#pragma once

#include <wrl.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;

namespace GameSmith {
	// This class is a wrapper over DX12 command lists to avoid header inclusion
	class DirectX12CommandListWrapper
	{
	public:
		friend class DirectX12CommandContextBase;
		DirectX12CommandListWrapper(ComPtr<ID3D12GraphicsCommandList6> rawList);
		bool isOpen() { return m_IsOpen; }
		inline ID3D12GraphicsCommandList6* operator ->  () { return m_RawList.Get(); }
		inline ID3D12GraphicsCommandList6* operator& () {return m_RawList.Get();}
	private:
		void CloseList();
	private:
		ComPtr<ID3D12GraphicsCommandList6> m_RawList;
		bool m_IsOpen;
	};

};

