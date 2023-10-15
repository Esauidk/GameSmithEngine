#include "gepch.h"
#include "DirectX12RootSignature.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

#include "ProjectGE/Rendering/DirectX12/Util/DirectX12RootSignatureBuilder.h"


namespace ProjectGE {
	void DirectX12RootSignature::Init(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc)
	{
		auto& core = DirectX12Core::GetCore();
		auto pDevice = core.GetDevice();
		m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &m_RootSigFeat, sizeof(m_RootSigFeat)))) {
			m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
		bool res = FAILED(D3DX12SerializeVersionedRootSignature(&desc, m_RootSigFeat.HighestVersion, &m_RootSigBlob, &m_ErrorBlob));
		GE_CORE_ASSERT(!res, "Failed to serialized root signature");

		res = FAILED(pDevice->CreateRootSignature(0, m_RootSigBlob->GetBufferPointer(), m_RootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_Root)));
		GE_CORE_ASSERT(!res, "Failed to create root signature");

		IndexRootSignature(desc);
	}

	void DirectX12RootSignature::InitGenericRootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags, bool tesselation)
	{
		DirectX12RootSignatureBuilder builder;
		if (tesselation) {
			CreateLargeTesGraphicsRootSignature(builder, flags);
		}
		else {
			CreateLargeGraphicsRootSignature(builder, flags);
		}
		
		Init(builder.Build());
	}

	void DirectX12RootSignature::IndexRootSignature(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc)
	{
		// Analysis method taken from Unreal Engine:
		// Expected Layout of Root Signature is as follows:
		// EACH STAGE WILL HAVE POTENTIALLY 4 TYPES OF PARAMETERS:
		// SRV TABLES, CBV TABLES, ROOT CONSTANTS, AND SAMPLERS
		// 
		// THERE IS ALSO THE CHANCE OF PARAMETERS THAT BELONG TO ALL STAGES
		// GLOBAL SRV TABLE, GLOBAL CBV TABLE, GLOBAL ROOT CONSTANTS, GLOBAL SAMPLERS, AND UAVS
		// 
		// The goal is to have a mapping of register entries for each type of parameter for all kinds of visibility. 
		// Anyone can then, prompt for the index before attempting to bind in that slot
		// 
		// Another expectation is that the root signature parameter register index should be in the order of the parameters in the description
		// 
		// Steps for analysis:
		// 1) Determine The total size of the root signature (with each parameter offset)
		// 2) Determine the accessibility of the root signature (which stages can access)
		// 3) Determine the size and binding slots of the resources
		const bool denyVS = desc.Desc_1_1.Flags & D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
		const bool denyPS = desc.Desc_1_1.Flags & D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		for (UINT i = 0; i < desc.Desc_1_1.NumParameters; i++) {
			
			auto& curParam = desc.Desc_1_1.pParameters[i];

			// Figure out which stage this parameter is for
			Stages stage = STAGE_NUM;
			switch (curParam.ShaderVisibility) {
			case D3D12_SHADER_VISIBILITY_ALL:
				stage = STAGE_NUM;
				break;
			case D3D12_SHADER_VISIBILITY_VERTEX:
				stage = STAGE_VERTEX;
				m_StageInfo[stage].visibleStage = m_StageInfo[stage].visibleStage || !denyVS;
				break;
			case D3D12_SHADER_VISIBILITY_PIXEL:
				stage = STAGE_PIXEL;
				m_StageInfo[stage].visibleStage = m_StageInfo[stage].visibleStage || !denyPS;
				break;
			}


			switch (curParam.ParameterType) {
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
			{
				GE_CORE_ASSERT(curParam.DescriptorTable.NumDescriptorRanges == 1, "Descriptor table in root signature has more than one range: UNEXPECTD");
				auto& range = curParam.DescriptorTable.pDescriptorRanges[0];
				switch (range.RangeType) {
				case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
					SetCBVRegisterSlot(stage, i);
					SetCBVMaxCount(stage, range.NumDescriptors);
					break;
				case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
					SetSamplerRegisterSlot(stage, i);
					SetSamplerMaxCount(stage, range.NumDescriptors);
					break;
				case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
					SetSRVRegisterSlot(stage, i);
					SetSRVMaxCount(stage, range.NumDescriptors);
					break;
				case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
					SetUAVRegisterSlot(stage, i);
					SetUAVMaxCount(stage, range.NumDescriptors);
					break;
				}
				break;
			}
			case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
				//TODO: ADD IN LOGIC FOR CONSTANTS
				break;
			default:
				GE_CORE_ASSERT(false, "All descriptors besides root CBVs should be in a table");
			}
		}
	}

	/*void DirectX12RootSignature::Bind() {
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->SetGraphicsRootSignature(m_Root.Get());
	}*/
};

