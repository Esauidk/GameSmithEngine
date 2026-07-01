#pragma once
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12DedicatedBuffer.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/ConstantBuffers/DirectX12ConstantBuffer.h"


namespace GameSmith {
	class DirectX12VRAMConstantBuffer : public DirectX12ConstantBuffer
	{
	public:
		DirectX12VRAMConstantBuffer(BYTE* data, UINT size); 
		DirectX12VRAMConstantBuffer(UINT size);
		DirectX12VRAMConstantBuffer(UINT size, std::string name);
		void UpdateData(BYTE* data, UINT byteSize) override;
		virtual unsigned int GetBufferSize() override { return m_Buffer->GetSize(); };
		D3D12_GPU_VIRTUAL_ADDRESS& GetGPUReference();
		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor();
	private:
		void GenerateConstantBuffView();
	private:
		Scope<DirectX12DedicatedBuffer<BYTE>> m_Buffer;
		D3D12_GPU_VIRTUAL_ADDRESS m_GPUAdd;
		DirectX12LoaderDescriptor m_TempDescriptor;
	};
};


