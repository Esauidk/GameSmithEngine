#include "gepch.h"
#include "DirectX12StateManager.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

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

	void DirectX12StateManager::BindState()
	{
		LowLevelSetRootSignature(PipelineState.Graphics.CurPipelineData->m_Root);
		LowLevelSetGraphicsPipelineState(PipelineState.Graphics.CurPipelineData->m_Pso);
	}

	void DirectX12StateManager::LowLevelSetGraphicsPipelineState(Ref<DirectX12PipelineState> pipeline)
	{
		PipelineState.Basic.CurPipeline = pipeline;

		auto& core = DirectX12Core::GetCore();
		auto& commandList = core.GetDirectCommandContext()->GetCommandList();
		commandList->SetPipelineState(pipeline->GetPipelineState());
	}

	void DirectX12StateManager::LowLevelSetRootSignature(Ref<DirectX12RootSignature> root)
	{
		if (PipelineState.Graphics.updateRootSignature) {
			auto& core = DirectX12Core::GetCore();
			auto& commandList = core.GetDirectCommandContext()->GetCommandList();
			commandList->SetGraphicsRootSignature(root->GetInternalRootSignature());
			PipelineState.Graphics.updateRootSignature = false;
		}
	}
};

