#pragma once

#include <d3d12.h>

namespace ProjectGE {
	class DirectX12View {
	public:
		DirectX12View(D3D12_CPU_DESCRIPTOR_HANDLE view) : m_View(view) {}
		inline const D3D12_CPU_DESCRIPTOR_HANDLE GetView() const { return m_View; }
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE m_View;
	};

	class DirectX12RenderTargetView : public DirectX12View {
	public:
		DirectX12RenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE view, DXGI_FORMAT format) : DirectX12View(view), m_Format(format) {}
		inline const DXGI_FORMAT GetFormat() const { return m_Format; }
	private:
		DXGI_FORMAT m_Format;
	};

	class DirectX12DepthTargetView : public DirectX12View {
	public:
		DirectX12DepthTargetView(D3D12_CPU_DESCRIPTOR_HANDLE view) : DirectX12View(view) {}
	};

	class DirectX12ShaderResourceView : public DirectX12View {
	public:
		DirectX12ShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE view) : DirectX12View(view) {};

	};
};
