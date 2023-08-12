#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ConstantBuffer.h"
#include "DirectX12Buffer.h"


namespace ProjectGE {
	class DirectX12ConstantBuffer : public DirectX12Buffer<BYTE>, public ConstantBuffer
	{
	public:
		DirectX12ConstantBuffer(BYTE* data, UINT size) : DirectX12Buffer<BYTE>(data, size, "Constant Buffer") { m_GPUAddress = m_GpuBuffer->GetGPUVirtualAddress(); }
		DirectX12ConstantBuffer(UINT size) : DirectX12Buffer<BYTE>(size, "Constant Buffer") { m_GPUAddress = m_GpuBuffer->GetGPUVirtualAddress(); }
		void UpdateData(BYTE* data) override;
		void* GetGPUReference() override;
	private:
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUAddress;
	};
};


