#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ConstantBuffer.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Buffer.h"


namespace ProjectGE {
	class DirectX12ConstantBuffer : public ConstantBuffer
	{
	public:
		DirectX12ConstantBuffer(BYTE* data, UINT size) : m_Buffer(std::make_unique<DirectX12Buffer<BYTE>>(data, size, "Constant Buffer")) { m_GPUAdd = m_Buffer->GetGPUReference(); }
		DirectX12ConstantBuffer(UINT size) :m_Buffer(std::make_unique<DirectX12Buffer<BYTE>>(size, "Constant Buffer")) { m_GPUAdd = m_Buffer->GetGPUReference(); }
		void UpdateData(BYTE* data) override;
		void* GetGPUReference() override;
	private:
		Scope<DirectX12Buffer<BYTE>> m_Buffer;
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUAdd;
	};
};


