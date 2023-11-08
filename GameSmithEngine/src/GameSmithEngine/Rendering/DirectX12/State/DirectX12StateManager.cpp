#include "gepch.h"
#include "DirectX12StateManager.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"

#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	DirectX12StateManager::DirectX12StateManager(DirectX12QueueType cmdType) : m_HeapState(cmdType), m_StateType(cmdType) {
		GE_CORE_ASSERT(cmdType != DirectX12QueueType::Copy, "We do not keep state of copy command lists");
	}

	void DirectX12StateManager::SetGraphicsPipelineState(Ref<DirectX12GraphicsPipelineState> pipelineData)
	{
		auto currentPipelineData = PipelineState.Graphics.CurPipelineData;
		bool isNull = currentPipelineData.get() == nullptr;
		if (isNull || currentPipelineData.get() != pipelineData.get()) {
			if (isNull || currentPipelineData->m_Root.get() != pipelineData->m_Root.get()) {
				updateRootSignature = true;
			}

			PipelineState.Graphics.CurPipelineData = pipelineData;
			LowLevelSetGraphicsPipelineState(pipelineData->m_Pso);
		}
		
	}

	void DirectX12StateManager::SetRenderTargets(DirectX12RenderTargetView** target, UINT number, DirectX12DepthTargetView depth)
	{
		for (UINT i = 0; i < number; i++) {
			PipelineState.Graphics.RenderTargets[i] = target[i];
		}

		PipelineState.Graphics.depthTarget = depth;
		PipelineState.Graphics.numRenderTargets = number;
		updateRenderTargets = true;
	}

	void DirectX12StateManager::NewCommandList()
	{
		updateRenderTargets = true;
		updateRootSignature = true;

		if (PipelineState.Graphics.curVBuff.BufferLocation != NULL) {
			updateVertexData = true;
		}

		if (PipelineState.Graphics.curIBuff.BufferLocation != NULL) {
			updateIndexData = true;
		}

		if (PipelineState.Graphics.topListType != D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED) {
			setTop = true;
		}

		if (PipelineState.Graphics.numRects > 0) {
			setRects = true;
		}

		if (PipelineState.Graphics.numViewports > 0) {
			setViewports = true;
		}

		PipelineState.Basic.CBVStorage.SetDirtyAll();
		PipelineState.Basic.SRVStorage.SetDirtyAll();
		PipelineState.Basic.SamplerStorage.SetDirtyAll();
		
		m_HeapState.AttachHeap();
	}

	void DirectX12StateManager::NewDescriptorHeap()
	{
		PipelineState.Basic.CBVStorage.SetDirtyAll();
		PipelineState.Basic.SRVStorage.SetDirtyAll();
		PipelineState.Basic.SamplerStorage.SetDirtyAll();
	}

	void DirectX12StateManager::BindState()
	{
		LowLevelSetRootSignature(PipelineState.Graphics.CurPipelineData->m_Root);
		LowLevelSetGraphicsPipelineState(PipelineState.Graphics.CurPipelineData->m_Pso);

		if (updateResources) {
			SetResources(STAGE_VERTEX, STAGE_NUM);
		}

		if (updateSamplers) {
			SetSamplers(STAGE_VERTEX, STAGE_NUM);
		}

		auto& core = DirectX12Core::GetCore();

		auto& commandList = core.GetDirectCommandContext().GetCommandList();

		if (updateRenderTargets) {
			D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetArray[MAX_SIM_RENDER_TARGETS] = {};
			D3D12_CPU_DESCRIPTOR_HANDLE DepthTarget = PipelineState.Graphics.depthTarget.m_View;

			for (UINT i = 0; i < PipelineState.Graphics.numRenderTargets; i++) {
				RenderTargetArray[i] = PipelineState.Graphics.RenderTargets[i]->m_View;
			}
			commandList->OMSetRenderTargets(PipelineState.Graphics.numRenderTargets, RenderTargetArray, 0, &DepthTarget);
			updateRenderTargets = false;
		}

		

		if (updateVertexData) {
			commandList->IASetVertexBuffers(0, 1, &PipelineState.Graphics.curVBuff);
			updateVertexData = false;
		}

		if (updateIndexData) {
			commandList->IASetIndexBuffer(&PipelineState.Graphics.curIBuff);
			updateIndexData = false;
		}

		if (setTop) {
			commandList->IASetPrimitiveTopology(PipelineState.Graphics.topListType);
			setTop = false;
		}

		if (setViewports) {
			commandList->RSSetViewports(PipelineState.Graphics.numViewports, PipelineState.Graphics.viewport);
			setViewports = false;
		}

		if (setRects) {
			commandList->RSSetScissorRects(PipelineState.Graphics.numRects, PipelineState.Graphics.rects);
			setRects = false;
		}
	}

	void DirectX12StateManager::BindRenderTargetsOnly()
	{
		auto& core = DirectX12Core::GetCore();

		auto& commandList = core.GetDirectCommandContext().GetCommandList();

		if (updateRenderTargets) {
			D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetArray[MAX_SIM_RENDER_TARGETS] = {};
			D3D12_CPU_DESCRIPTOR_HANDLE DepthTarget = PipelineState.Graphics.depthTarget.m_View;

			for (UINT i = 0; i < PipelineState.Graphics.numRenderTargets; i++) {
				RenderTargetArray[i] = PipelineState.Graphics.RenderTargets[i]->m_View;
			}
			commandList->OMSetRenderTargets(PipelineState.Graphics.numRenderTargets, RenderTargetArray, 0, &DepthTarget);
			updateRenderTargets = false;
		}
	}

	void DirectX12StateManager::SetVBV(D3D12_VERTEX_BUFFER_VIEW& newBuffer)
	{
		GE_CORE_ASSERT(newBuffer.BufferLocation != NULL, "Invalid vertex buffer GPU memory location");
		PipelineState.Graphics.curVBuff = newBuffer;
		updateVertexData = true;
	}

	void DirectX12StateManager::SetIBV(D3D12_INDEX_BUFFER_VIEW& newBuffer)
	{
		GE_CORE_ASSERT(newBuffer.BufferLocation != NULL, "Invalid index buffer GPU memory location");
		PipelineState.Graphics.curIBuff = newBuffer;
		updateIndexData = true;
	}

	void DirectX12StateManager::SetTop(D3D12_PRIMITIVE_TOPOLOGY& listType)
	{
		GE_CORE_ASSERT(listType != D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED, "Not a defined topology list");
		PipelineState.Graphics.topListType = listType;
		setTop = true;
	}


	void DirectX12StateManager::SetSRV(Stages stage, DirectX12ShaderResourceView view, UINT index)
	{
		PipelineState.Basic.SRVStorage.Views[stage][index] = view;

		SRVStorage::SetSlotDirty(PipelineState.Basic.SRVStorage.Dirty[stage], index);
		updateResources = true;
	}

	void DirectX12StateManager::SetCBV(Stages stage, DirectX12ConstantBufferView view, UINT index)
	{
		PipelineState.Basic.CBVStorage.ResourceLocations[stage][index] = view.m_GPUAdd;
		PipelineState.Basic.CBVStorage.Descriptors[stage][index] = view.m_View;

		CBVStorage::SetSlotDirty(PipelineState.Basic.CBVStorage.Dirty[stage], index);

		updateResources = true;
	}

	void DirectX12StateManager::SetSampler(Stages stage, DirectX12SamplerView view, UINT index)
	{
		PipelineState.Basic.SamplerStorage.Views[stage][index] = view;

		SamplerStorage::SetSlotDirty(PipelineState.Basic.SamplerStorage.Dirty[stage], index);
		updateSamplers = true;
	}

	void DirectX12StateManager::SetRects(D3D12_RECT* rects, UINT count)
	{
		for (UINT i = 0; i < count; i++) {
			PipelineState.Graphics.rects[i] = rects[i];
		}

		PipelineState.Graphics.numRects = count;
		setRects = true;
	}

	void DirectX12StateManager::SetViewports(D3D12_VIEWPORT* viewports, UINT count)
	{
		for (UINT i = 0; i < count; i++) {
			PipelineState.Graphics.viewport[i] = viewports[i];
		}

		PipelineState.Graphics.numViewports = count;
		setViewports = true;
	}
	/*

	void DirectX12StateManager::SetUAV(Stages stage, D3D12_CPU_DESCRIPTOR_HANDLE* views, UINT viewCount)
	{
		PipelineState.Basic.UAVStorage.Views[stage] = views;
		PipelineState.Basic.UAVStorage.numberViews[stage] = viewCount;

		PipelineState.Basic.updateResources = true;
	}
	*/
	void DirectX12StateManager::LowLevelSetGraphicsPipelineState(Ref<DirectX12PipelineState> pipeline)
	{
		PipelineState.Basic.CurPipeline = pipeline;

		auto& core = DirectX12Core::GetCore();
		auto& commandList = core.GetDirectCommandContext().GetCommandList();
		commandList->SetPipelineState(pipeline->GetPipelineState());
	}

	void DirectX12StateManager::LowLevelSetRootSignature(Ref<DirectX12RootSignature> root)
	{
		if (updateRootSignature) {
			auto& core = DirectX12Core::GetCore();
			auto& commandList = core.GetDirectCommandContext().GetCommandList();
			commandList->SetGraphicsRootSignature(root->GetInternalRootSignature());
			updateRootSignature = false;
		}
	}


	void DirectX12StateManager::SetResources(Stages beginStage, Stages endStage)
	{
		UINT begin = (UINT)beginStage;
		UINT end = (UINT)endStage;

		GE_CORE_ASSERT(begin < end, "Invalid stage range");

		UINT numSRV[Stages::STAGE_NUM];
		UINT numCBV[Stages::STAGE_NUM];
		UINT numViews = 0;
		UINT heapSlot = 0;

		DirectX12RootSignature& root = *(PipelineState.Graphics.CurPipelineData->m_Root.get());
		bool retryBinding = true;

		while (retryBinding) {
			retryBinding = false;

			for (UINT i = begin; i < end; i++) {
				Stages cur = (Stages)i;

				if (PipelineState.Basic.SRVStorage.Dirty[cur]) {
					numSRV[cur] = root.GetMaxSRV(cur);

					numViews += numSRV[cur];
				}
				else {
					numSRV[cur] = 0;
				}

				if (PipelineState.Basic.CBVStorage.Dirty[cur]) {

					numCBV[cur] = root.GetMaxCBV(cur);

					numViews += numCBV[cur];
				}
				else {
					numCBV[cur] = 0;
				}
			}

			if (!m_HeapState.CanFitView(numViews)) {
				m_HeapState.ReallocateViewHeap(numViews);
				retryBinding = true;
				numViews = 0;
				continue;
			}

			heapSlot = m_HeapState.GetFreeViewSlots(numViews);

		}
		
		SRVStorage& srvStorage = PipelineState.Basic.SRVStorage;
		for (UINT i = begin; i <= end; i++) {
			Stages cur = (Stages)i;

			if (srvStorage.Dirty[cur]) {
				m_HeapState.SetSRV(cur, root, srvStorage, numSRV[cur], heapSlot);
			}
		}

		CBVStorage& cbvStorage = PipelineState.Basic.CBVStorage;
		for (UINT i = begin; i <= end; i++) {
			Stages cur = (Stages)i;

			if (cbvStorage.Dirty[cur]) {
				m_HeapState.SetCBV(cur, root, cbvStorage, numCBV[cur], heapSlot);
			}
		}
	}

	void DirectX12StateManager::SetSamplers(Stages beginStage, Stages endStage)
	{

		UINT begin = (UINT)beginStage;
		UINT end = (UINT)endStage;

		GE_CORE_ASSERT(begin < end, "Invalid stage range");

		UINT numSampler[Stages::STAGE_NUM];
		UINT numSamplers = 0;
		UINT heapSlot = 0;

		DirectX12RootSignature& root = *(PipelineState.Graphics.CurPipelineData->m_Root.get());
		bool retryBinding = true;

		while (retryBinding) {
			retryBinding = false;

			for (UINT i = begin; i < end; i++) {
				Stages cur = (Stages)i;

				if (PipelineState.Basic.SamplerStorage.Dirty[cur]) {
					numSampler[cur] = root.GetMaxSampler(cur);

					numSamplers += numSampler[cur];
				}
				else {
					numSampler[cur] = 0;
				}
			}

			if (!m_HeapState.CanFitSampler(numSamplers)) {
				m_HeapState.ReallocateSamplerHeap(numSamplers);
				retryBinding = true;
				numSamplers = 0;
				continue;
			}

			heapSlot = m_HeapState.GetFreeSamplerSlots(numSamplers);

		}

		SamplerStorage& samplerStorage = PipelineState.Basic.SamplerStorage;
		for (UINT i = begin; i < end; i++) {
			Stages cur = (Stages)i;

			if (samplerStorage.Dirty[cur]) {
				m_HeapState.SetSampler(cur, root, samplerStorage, numSampler[cur], heapSlot);
			}
		}
	}
};

