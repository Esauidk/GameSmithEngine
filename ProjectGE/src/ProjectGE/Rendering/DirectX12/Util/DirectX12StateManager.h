#pragma once


#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12RootSignature.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Views.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12HeapDescriptorState.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Macos.h"

namespace ProjectGE {

	template<typename SlotMask>
	struct ViewStorage {
		SlotMask Dirty[STAGE_NUM];

		static inline void SetSlotDirty(SlotMask& slotMask, UINT slot) {
			slotMask |= (1 << slot);
		}

		inline void SetDirtyStage(Stages stage, SlotMask slotMask = (SlotMask)-1) {
			Dirty[stage] |= slotMask;
		}

		inline void SetDirtyAll(SlotMask slotMask = (SlotMask)-1) {
			SetDirtyStage(STAGE_VERTEX, slotMask);
			SetDirtyStage(STAGE_PIXEL, slotMask);
		}

		static inline void SetSlotClean(SlotMask& slotMask, UINT slot) {
			slotMask &= ~(1 << slot);
		}

		static inline bool IsSlotDirty(SlotMask& slotMask, UINT slot) {
			return (slotMask & (1 << slot)) != 0;
		}

	};

	struct CBVStorage : ViewStorage<CBVSlotMask> {
		D3D12_GPU_VIRTUAL_ADDRESS ResourceLocations[STAGE_NUM][MAX_CBV] = { NULL };
		D3D12_CPU_DESCRIPTOR_HANDLE Descriptors[STAGE_NUM][MAX_CBV] = { NULL };
	};

	struct SRVStorage : ViewStorage<SRVSlotMask> {
		DirectX12ShaderResourceView Views[STAGE_NUM][MAX_SRV] = { NULL };
	};

	class DirectX12StateManager
	{
	public:
		DirectX12StateManager(DirectX12QueueType cmdType);
		Ref<DirectX12PipelineStateData> GetGraphicsPiplineState() { return PipelineState.Graphics.CurPipelineData; }
		void SetGraphicsPipelineState(Ref<DirectX12PipelineStateData> pipelineData);
		void SetRenderTarget(DirectX12RenderTargetView** target, UINT number, DirectX12DepthTargetView depth);
		void NewCommandList();
		void NewDescriptorHeap();
		void BindState();

		void SetVBV(D3D12_VERTEX_BUFFER_VIEW& newBuffer);
		void SetIBV(D3D12_INDEX_BUFFER_VIEW& newBuffer);
		void SetTop(D3D12_PRIMITIVE_TOPOLOGY& listType);

		void SetSRV(Stages stage, DirectX12ShaderResourceView view, UINT index);
		void SetCBV(Stages stage, DirectX12ConstantBufferView view, UINT index);
		//void SetUAV(Stages stage, D3D12_CPU_DESCRIPTOR_HANDLE* views, UINT viewCount);
	private:
		void LowLevelSetGraphicsPipelineState(Ref<DirectX12PipelineState> pipeline);
		void LowLevelSetRootSignature(Ref<DirectX12RootSignature> root);
		void SetResources(Stages beginStage, Stages endStage);


		struct {
			struct {
				Ref<DirectX12PipelineStateData> CurPipelineData = nullptr;
				bool updateRootSignature;

				UINT numRenderTargets = 0;
				DirectX12RenderTargetView* RenderTargets[MAX_SIM_RENDER_TARGETS] = {};
				DirectX12DepthTargetView depthTarget;
				bool updateRenderTargets;

				D3D12_VERTEX_BUFFER_VIEW curVBuff;
				bool updateVertexData;

				D3D12_INDEX_BUFFER_VIEW curIBuff;
				bool updateIndexData;

				D3D12_PRIMITIVE_TOPOLOGY topListType;
				bool setTop;

			} Graphics = {};

			struct {
				CBVStorage CBVStorage;
				SRVStorage SRVStorage;
				//ViewStorage UAVStorage;
				bool updateResources;

				Ref<DirectX12PipelineState> CurPipeline = nullptr;
			} Basic = {};
			
		} PipelineState = {};

		DirectX12HeapDescriptorState m_HeapState;
		DirectX12QueueType m_StateType;
	};
};


