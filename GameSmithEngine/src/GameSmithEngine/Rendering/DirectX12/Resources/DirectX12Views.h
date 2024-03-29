#pragma once

#include <d3d12.h>

namespace GameSmith {
	
	struct DirectX12View {
		D3D12_CPU_DESCRIPTOR_HANDLE m_View;
	};

	struct DirectX12DefaultViews {
		DirectX12View EmptyCBV;
		DirectX12View EmptySRV;
		DirectX12View EmptySampler;
	};

	struct DirectX12RenderTargetView : public DirectX12View {
		DXGI_FORMAT m_Format;
	};

	struct DirectX12SamplerView : public DirectX12View {
	};

	struct DirectX12DepthTargetView : public DirectX12View {
	};

	struct DirectX12ShaderResourceView : public DirectX12View {
	};

	struct DirectX12ConstantBufferView : public DirectX12View {
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUAdd;
	};
};
