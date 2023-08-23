#pragma once
#include "gepch.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"


using Microsoft::WRL::ComPtr;

/* 
This class is a encapulated version of the Root Signature (An object linking paramters to stages in the render pipeline)
Job:
	- Collects flags and parameters
	- Serializes and Build the Root Signature
	- Binds it to the pipeline
*/
namespace ProjectGE {

	enum ResourceStages {
		VS_SRV,
		VS_CBV,
		VS_SAMPLE,
		VS_ROOTC,
		PS_SRV,
		PS_CBV,
		PS_SAMPLE,
		PS_ROOTC,
		ALL_SRV,
		ALL_CBV,
		ALL_UAV,
		ALL_ROOTC,
		ALL_SAMPLE,
		COUNT_ENUM
	};

	// TODO: Temporary, move to a more higher level file
	enum Stages {
		STAGE_VERTEX,
		STAGE_PIXEL,
		STAGE_NUM
	};

	// TODO: ADD FIELD FOR IF THE STAGE CAN EVEN SEE THE ROOT SIGNATURE
	struct StageMetadata {
		UINT maxSRV = 0;
		UINT maxCBV = 0;
		UINT maxSample = 0;
		UINT maxUAV = 0;
		bool visibleStage = false;
	};

	class DirectX12RootSignature
	{
	public: 
		DirectX12RootSignature() = default;
		void Init(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc);
		void InitGenericRootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags);
		inline ID3D12RootSignature* GetInternalRootSignature() { return m_Root.Get(); }

		inline bool HasResource() { return m_HasSRV || m_HasCBV || m_HasUAV; }
		inline bool HasSamplers() { return m_HasSample; }
		inline bool HasSRV() { return m_HasSRV; }
		inline bool HasCBV() { return m_HasCBV; }
		inline bool HasSample() {return m_HasSample;}
		inline bool HasUAV() { return m_HasUAV; }

		inline bool VSVisible() { return m_StageInfo[STAGE_VERTEX].visibleStage; }
		inline bool PSVisible() { return m_StageInfo[STAGE_PIXEL].visibleStage; }

		inline UINT GetMaxSRV(Stages stage) { return m_StageInfo[stage].maxSRV; }
		inline UINT GetMaxCBV(Stages stage) { return m_StageInfo[stage].maxCBV; }
		inline UINT GetMaxSampler(Stages stage) { return m_StageInfo[stage].maxSample; }
			//void Bind() override;
	private:
		void IndexRootSignature(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc);

		// TODO: ADD VARIABLES FOR HAVING RESOURCES GO THROUGH ALL SET SLOTS AND ACTIVATE
		inline void SetSRVRegisterSlot(Stages stage, UINT index) {
			switch (stage) {
			case STAGE_VERTEX:
				m_RegisterSlotTable[VS_SRV] = index;
				break;
			case STAGE_PIXEL:
				m_RegisterSlotTable[PS_SRV] = index;
				break;
			default:
				m_RegisterSlotTable[ALL_SRV] = index;
				break;
			}
		}

		inline void SetCBVRegisterSlot(Stages stage, UINT index) {
			switch (stage) {
			case STAGE_VERTEX:
				m_RegisterSlotTable[VS_CBV] = index;
				break;
			case STAGE_PIXEL:
				m_RegisterSlotTable[PS_CBV] = index;
				break;
			default:
				m_RegisterSlotTable[ALL_CBV] = index;
				break;
			}
		}

		inline void SetSamplerRegisterSlot(Stages stage, UINT index) {
			switch (stage) {
			case STAGE_VERTEX:
				m_RegisterSlotTable[VS_SAMPLE] = index;
				break;
			case STAGE_PIXEL:
				m_RegisterSlotTable[PS_SAMPLE] = index;
				break;
			default:
				m_RegisterSlotTable[ALL_SAMPLE] = index;
				break;
			}
		}

		inline void SetUAVRegisterSlot(Stages stage, UINT index) {
			m_RegisterSlotTable[ALL_UAV] = index;
		}

		inline void SetSRVMaxCount(Stages stage, UINT count) {
			if (!m_HasSRV) {
				m_HasSRV = count > 0;
			}

			if (stage == STAGE_NUM) {
				for (UINT i = STAGE_VERTEX; i < STAGE_NUM; i++) {
					m_StageInfo[i].maxSRV = count;
				}
			}
			else {
				m_StageInfo[stage].maxSRV = count;
			}
		}

		inline void SetCBVMaxCount(Stages stage, UINT count) {
			if (!m_HasCBV) {
				m_HasCBV = count > 0;
			}

			if (stage == STAGE_NUM) {
				for (UINT i = STAGE_VERTEX; i < STAGE_NUM; i++) {
					m_StageInfo[i].maxCBV = count;
				}
			}
			else {
				m_StageInfo[stage].maxCBV = count;
			}
		}

		inline void SetSamplerMaxCount(Stages stage, UINT count) {
			if (!m_HasSample) {
				m_HasSample = count > 0;
			}

			if (stage == STAGE_NUM) {
				for (UINT i = STAGE_VERTEX; i < STAGE_NUM; i++) {
					m_StageInfo[i].maxSample = count;
				}
			}
			else {
				m_StageInfo[stage].maxSample = count;
			}
		}

		inline void SetUAVMaxCount(Stages stage, UINT count) {
			if (!m_HasUAV) {
				m_HasUAV = count > 0;
			}
			
			if (stage == STAGE_NUM) {
				for (UINT i = STAGE_VERTEX; i < STAGE_NUM; i++) {
					m_StageInfo[i].maxUAV = count;
				}
			}
			else {
				m_StageInfo[stage].maxUAV = count;
			}
		}
	private:
		D3D12_FEATURE_DATA_ROOT_SIGNATURE m_RootSigFeat;
		ComPtr<ID3D12RootSignature> m_Root;
		UINT m_AvailableSlot;
		Microsoft::WRL::ComPtr<ID3DBlob> m_RootSigBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ErrorBlob;

		UINT m_RegisterSlotTable[COUNT_ENUM];
		StageMetadata m_StageInfo[STAGE_NUM];

		bool m_HasSRV;
		bool m_HasCBV;
		bool m_HasSample;
		bool m_HasUAV;
	};
}


