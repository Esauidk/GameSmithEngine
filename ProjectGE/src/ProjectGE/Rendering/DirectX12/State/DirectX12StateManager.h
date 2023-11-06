#pragma once
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12PipelineState.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12RootSignature.h"
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12Views.h"
#include "ProjectGE/Rendering/DirectX12/State/DirectX12HeapDescriptorState.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Macos.h"

namespace ProjectGE {

	template<typename SlotMask>
	struct ViewStorage {
		SlotMask Dirty[STAGE_NUM+1];

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
		D3D12_GPU_VIRTUAL_ADDRESS ResourceLocations[STAGE_NUM+1][MAX_CBV] = { NULL };
		D3D12_CPU_DESCRIPTOR_HANDLE Descriptors[STAGE_NUM+1][MAX_CBV] = { NULL };
	};

	struct SRVStorage : ViewStorage<SRVSlotMask> {
		DirectX12ShaderResourceView Views[STAGE_NUM+1][MAX_SRV] = { NULL };
	};

	struct SamplerStorage : ViewStorage<SamplerSlotMask> {
		DirectX12SamplerView Views[STAGE_NUM + 1][MAX_SAMPLER] = { NULL };
	};

	class DirectX12StateManager
	{
	public:
		DirectX12StateManager(DirectX12QueueType cmdType);
		Ref<DirectX12GraphicsPipelineState> GetGraphicsPiplineState() { return PipelineState.Graphics.CurPipelineData; }
		void SetGraphicsPipelineState(Ref<DirectX12GraphicsPipelineState> pipelineData);
		void SetRenderTargets(DirectX12RenderTargetView** target, UINT number, DirectX12DepthTargetView depth);
		void NewCommandList();
		void NewDescriptorHeap();
		void BindState();
		void BindRenderTargetsOnly();

		void SetVBV(D3D12_VERTEX_BUFFER_VIEW& newBuffer);
		void SetIBV(D3D12_INDEX_BUFFER_VIEW& newBuffer);
		void SetTop(D3D12_PRIMITIVE_TOPOLOGY& listType);

		void SetSRV(Stages stage, DirectX12ShaderResourceView view, UINT index);
		void SetCBV(Stages stage, DirectX12ConstantBufferView view, UINT index);
		//void SetUAV(Stages stage, D3D12_CPU_DESCRIPTOR_HANDLE* views, UINT viewCount);
		void SetSampler(Stages stage, DirectX12SamplerView view, UINT index);

		void SetRects(D3D12_RECT* rects, UINT count);
		void SetViewports(D3D12_VIEWPORT* viewports, UINT count);
	private:
		void LowLevelSetGraphicsPipelineState(Ref<DirectX12PipelineState> pipeline);
		void LowLevelSetRootSignature(Ref<DirectX12RootSignature> root);
		void SetResources(Stages beginStage, Stages endStage);
		void SetSamplers(Stages beginStage, Stages endStage);

		bool updateRootSignature = true;
		bool updateRenderTargets = true;
		bool updateVertexData = true;
		bool updateIndexData = true;
		bool setTop = true;
		bool setViewports = true;
		bool setRects = true;
		bool updateResources = true;
		bool updateSamplers = true;

		struct {
			struct {
				Ref<DirectX12GraphicsPipelineState> CurPipelineData = nullptr;
				

				UINT numRenderTargets = 0;
				DirectX12RenderTargetView* RenderTargets[MAX_SIM_RENDER_TARGETS] = {};
				DirectX12DepthTargetView depthTarget;
				

				D3D12_VERTEX_BUFFER_VIEW curVBuff;
				

				D3D12_INDEX_BUFFER_VIEW curIBuff;
				

				D3D12_PRIMITIVE_TOPOLOGY topListType;
				

				UINT numViewports;
				D3D12_VIEWPORT viewport[MAX_RECT_AND_VIEWPORT] = {};

				UINT numRects;
				D3D12_RECT rects[MAX_RECT_AND_VIEWPORT] = {};

			} Graphics = {};

			struct {
				CBVStorage CBVStorage;
				SRVStorage SRVStorage;
				SamplerStorage SamplerStorage;
				//ViewStorage UAVStorage;

				Ref<DirectX12PipelineState> CurPipeline = nullptr;
			} Basic = {};
			
		} PipelineState = {};

		DirectX12HeapDescriptorState m_HeapState;
		DirectX12QueueType m_StateType;
	};
};


