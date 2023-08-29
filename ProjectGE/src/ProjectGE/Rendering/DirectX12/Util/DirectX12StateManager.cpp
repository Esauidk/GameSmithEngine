#include "gepch.h"
#include "DirectX12StateManager.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	void DirectX12StateManager::SetGraphicsPipelineState(Ref<DirectX12PipelineStateData> pipelineData)
	{
		auto currentPipelineData = PipelineState.Graphics.CurPipelineData;
		bool isNull = currentPipelineData.get() == nullptr;
		if (isNull || currentPipelineData.get() != pipelineData.get()) {
			if (isNull || currentPipelineData->m_Root.get() != pipelineData->m_Root.get()) {
				PipelineState.Graphics.updateRootSignature = true;
			}

			PipelineState.Graphics.CurPipelineData = pipelineData;
			LowLevelSetGraphicsPipelineState(pipelineData->m_Pso);
		}
		
	}

	void DirectX12StateManager::SetRenderTarget(Ref<DirectX12RenderTargetView>* target, UINT number, Ref<DirectX12DepthTargetView> depth)
	{
		PipelineState.Graphics.RenderTargets = target;
		PipelineState.Graphics.depthTarget = depth;
		PipelineState.Graphics.numRenderTargets = number;
		PipelineState.Graphics.updateRenderTargets = true;
	}

	void DirectX12StateManager::NewCommandList()
	{
		PipelineState.Graphics.updateRenderTargets = true;
		PipelineState.Graphics.updateRootSignature = true;
		m_HeapState.AttachHeap();
	}

	void DirectX12StateManager::BindState()
	{
		LowLevelSetRootSignature(PipelineState.Graphics.CurPipelineData->m_Root);
		LowLevelSetGraphicsPipelineState(PipelineState.Graphics.CurPipelineData->m_Pso);

		auto& core = DirectX12Core::GetCore();
		auto& commandList = core.GetDirectCommandContext().GetCommandList();

		if (PipelineState.Graphics.updateRenderTargets) {
			D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetArray[1];
			D3D12_CPU_DESCRIPTOR_HANDLE DepthTarget = PipelineState.Graphics.depthTarget->GetView();

			for (UINT i = 0; i < PipelineState.Graphics.numRenderTargets; i++) {
				RenderTargetArray[i] = PipelineState.Graphics.RenderTargets[i]->GetView();
			}
			commandList->OMSetRenderTargets(PipelineState.Graphics.numRenderTargets, RenderTargetArray, 0, &DepthTarget);
			PipelineState.Graphics.updateRenderTargets = false;
		}

		if (PipelineState.Basic.updateResources) {
			SetResources(STAGE_VERTEX, STAGE_NUM);
		}

	}

	void DirectX12StateManager::SetSRV(Stages stage, DirectX12ShaderResourceView* view, UINT index)
	{
		PipelineState.Basic.SRVStorage.Views[stage][index] = view;

		PipelineState.Basic.updateResources = true;
	}

	/*void DirectX12StateManager::SetCBV(Stages stage, D3D12_CPU_DESCRIPTOR_HANDLE* views, UINT viewCount)
	{
		PipelineState.Basic.CBVStorage.Views[stage] = views;
		PipelineState.Basic.CBVStorage.numberViews[stage] = viewCount;

		PipelineState.Basic.updateResources = true;
	}

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
		if (PipelineState.Graphics.updateRootSignature) {
			auto& core = DirectX12Core::GetCore();
			auto& commandList = core.GetDirectCommandContext().GetCommandList();
			commandList->SetGraphicsRootSignature(root->GetInternalRootSignature());
			PipelineState.Graphics.updateRootSignature = false;
		}
	}
	void DirectX12StateManager::SetResources(Stages beginStage, Stages endStage)
	{
		UINT begin = (UINT)beginStage;
		UINT end = (UINT)endStage;

		GE_CORE_ASSERT(begin < end, "Invalid stage range");

		for (UINT i = begin; i < end; i++) {
			Stages cur = (Stages)i;
			//TODO: Count the nubmer of resources in each array
			//m_HeapState.SetSRV(cur, *PipelineState.Graphics.CurPipelineData->m_Root, PipelineState.Basic.SRVStorage.Views[cur], PipelineState.Basic.SRVStorage.numberViews[cur]);
			//m_HeapState.SetCBV(cur, *PipelineState.Graphics.CurPipelineData->m_Root, PipelineState.Basic.CBVStorage.Views[cur], PipelineState.Basic.CBVStorage.numberViews[cur]);
		}
		
	}
};

