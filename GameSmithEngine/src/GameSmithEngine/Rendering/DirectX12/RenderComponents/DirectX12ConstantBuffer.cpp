#include "gepch.h"
#include "DirectX12ConstantBuffer.h"

#define CBV_ALIGNMENT 256

namespace GameSmith {
	static UINT AllignSize(UINT proposedSize) {
		UINT result = proposedSize;
		bool missing = (proposedSize % CBV_ALIGNMENT != 0);
		if (missing) {
			UINT div = proposedSize / CBV_ALIGNMENT;

			result += CBV_ALIGNMENT * (div + 1) - proposedSize;
		}

		return result;
	}

	void DirectX12ConstantBuffer::UpdateData(BYTE* data, UINT byteSize)
	{
		auto& core = DirectX12Core::GetCore();
		auto dirContext = core.GetDirectCommandContext();
		auto copyContext = core.GetCopyCommandContext();
		auto& stateTracker = m_Buffer->GetStateTracker();
		auto originalState = stateTracker.GetState();

		// Let the Direct queue know that this constant buffer is no longer usable
		if (stateTracker.TransitionBarrier(D3D12_RESOURCE_STATE_COMMON, dirContext)) {
			dirContext->FinalizeResourceBarriers();
			dirContext->FinalizeCommandList();
			dirContext->SubmitCommandLists();
		}

		// Let the Copy queue know that this constant buffer is now a copy destination
		if (stateTracker.TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST, copyContext)) {
			copyContext->FinalizeResourceBarriers();
		}

		m_Buffer->UpdateData(data, byteSize);
		// Tell the Direct queue to wait for copying to finish before executing more command lists
		m_Buffer->SetUploadGPUBlock();
		// Tell the Direct queue this constant buffer is back to its old state
		stateTracker.UndoTransition(copyContext);
		copyContext->FinalizeCommandList();
		copyContext->SubmitCommandLists();

		stateTracker.TransitionBarrier(originalState, dirContext);
		dirContext->FinalizeResourceBarriers();

	}

	D3D12_GPU_VIRTUAL_ADDRESS& DirectX12ConstantBuffer::GetGPUReference()
	{
		return m_GPUAdd;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12ConstantBuffer::GetDescriptor()
	{
		return m_TempDescriptor;
	}

	void DirectX12ConstantBuffer::GenerateConstantBuffView()
	{

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = m_GPUAdd;
		desc.SizeInBytes = m_Buffer->GetSize();

		DirectX12Core::GetCore().GetDevice()->CreateConstantBufferView(&desc, m_TempDescriptor);

		auto& core = DirectX12Core::GetCore();
		auto dirContext = core.GetDirectCommandContext();
		m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, dirContext);
		dirContext->FinalizeResourceBarriers();
	}

	DirectX12ConstantBuffer::DirectX12ConstantBuffer(BYTE* data, UINT size) : m_Buffer(Scope<DirectX12Buffer<BYTE>>(new DirectX12Buffer<BYTE>(data, AllignSize(size), "Constant Buffer")))
	{
		m_GPUAdd = m_Buffer->GetGPUReference();
		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
		GenerateConstantBuffView();
	}

	DirectX12ConstantBuffer::DirectX12ConstantBuffer(UINT size) : m_Buffer(Scope<DirectX12Buffer<BYTE>>(new DirectX12Buffer<BYTE>(AllignSize(size), "Constant Buffer")))
	{
		m_GPUAdd = m_Buffer->GetGPUReference();
		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
		GenerateConstantBuffView();
	}

	DirectX12ConstantBuffer::DirectX12ConstantBuffer(UINT size, std::string name) : m_Buffer(Scope<DirectX12Buffer<BYTE>>(new DirectX12Buffer<BYTE>(AllignSize(size), name)))
	{
		m_GPUAdd = m_Buffer->GetGPUReference();
		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
		GenerateConstantBuffView();
	}
};

