#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include <d3d12.h>

namespace GameSmith {
	class DirectX12ConstantBuffer : public ConstantBuffer
	{
	public:
		virtual D3D12_GPU_VIRTUAL_ADDRESS& GetGPUReference() = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor() = 0;
	};
};
