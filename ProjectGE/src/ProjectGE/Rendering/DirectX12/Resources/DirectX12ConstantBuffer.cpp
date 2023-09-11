#include "gepch.h"
#include "DirectX12ConstantBuffer.h"

#define CBV_ALIGNMENT 256

namespace ProjectGE {
	static UINT AllignSize(UINT proposedSize) {
		UINT result = proposedSize;
		bool missing = (proposedSize % CBV_ALIGNMENT != 0);
		if (missing) {
			UINT div = proposedSize / CBV_ALIGNMENT;

			result += CBV_ALIGNMENT * (div + 1) - proposedSize;
		}

		return result;
	}

	void ProjectGE::DirectX12ConstantBuffer::UpdateData(BYTE* data, UINT byteSize)
	{
		D3D12_RESOURCE_STATES originalState = m_Buffer->GetStateTracker().GetState();
		if (originalState != D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST);
		}

		m_Buffer->UpdateData(data, byteSize);

		if (m_Buffer->GetStateTracker().GetState() != originalState) {
			m_Buffer->SetUploadGPUBlock();
			m_Buffer->GetStateTracker().TransitionBarrier(originalState);
		}

	}

	D3D12_GPU_VIRTUAL_ADDRESS& ProjectGE::DirectX12ConstantBuffer::GetGPUReference()
	{
		if (m_Buffer->GetStateTracker().GetState() == D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Buffer->SetUploadGPUBlock();
			m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		}

		return m_GPUAdd;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12ConstantBuffer::GetDescriptor()
	{
		if (m_Buffer->GetStateTracker().GetState() == D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Buffer->SetUploadGPUBlock();
			m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		}

		return m_TempDescriptor;
	}

	void DirectX12ConstantBuffer::GenerateConstantBuffView()
	{

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = m_GPUAdd;
		desc.SizeInBytes = m_Buffer->GetSize();

		DirectX12Core::GetCore().GetDevice()->CreateConstantBufferView(&desc, m_TempDescriptor);
	}

	DirectX12ConstantBuffer::DirectX12ConstantBuffer(BYTE* data, UINT size) : m_Buffer(std::make_unique<DirectX12Buffer<BYTE>>(data, AllignSize(size), "Constant Buffer"))
	{
		m_GPUAdd = m_Buffer->GetGPUReference();
		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
		GenerateConstantBuffView();
	}

	DirectX12ConstantBuffer::DirectX12ConstantBuffer(UINT size) : m_Buffer(std::make_unique<DirectX12Buffer<BYTE>>(AllignSize(size), "Constant Buffer"))
	{
		m_GPUAdd = m_Buffer->GetGPUReference();
		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
		GenerateConstantBuffView();
	}
};

