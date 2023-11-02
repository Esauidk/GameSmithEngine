#pragma once
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12Buffer.h"
#include "ProjectGE/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"


namespace ProjectGE {
	class DirectX12ConstantBuffer : public ConstantBuffer
	{
	public:
		DirectX12ConstantBuffer(BYTE* data, UINT size); 
		DirectX12ConstantBuffer(UINT size);
		void UpdateData(BYTE* data, UINT byteSize) override;
		virtual unsigned int GetBufferSize() override { return m_Buffer->GetSize(); };
		D3D12_GPU_VIRTUAL_ADDRESS& GetGPUReference();
		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor();

	private:
		void GenerateConstantBuffView();
	private:
		Scope<DirectX12Buffer<BYTE>> m_Buffer;
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUAdd;
		DirectX12LoaderDescriptor m_TempDescriptor;

	};
};


