#pragma once

#include <d3d12.h>

namespace ProjectGE {
	class DirectX12RenderTargetView {
	public:
		DirectX12RenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE view, DXGI_FORMAT format) : m_View(view), m_Format(format) {}

		inline const D3D12_CPU_DESCRIPTOR_HANDLE GetView() const { return m_View; }
		inline const DXGI_FORMAT GetFormat() const { return m_Format; }
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE m_View;
		DXGI_FORMAT m_Format;
	};
};
