#include "gepch.h"
#include "DirectX12RootSignature.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"

#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12ShaderConstant.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12ShaderReference.h"


namespace ProjectGE {
	DirectX12RootSignature::DirectX12RootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags) : m_RootSigFeat(), m_Flags(flags), m_AvailableSlot(0) {
		auto pDevice = DirectX12Context::GetDevice();
		m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &m_RootSigFeat, sizeof(m_RootSigFeat)))) {
			m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
	}

	ShaderArguement* DirectX12RootSignature::AddArguement(UINT size, ShaderArguementType type)
	{
		DirectX12ShaderInput* input;
		switch (type) {
		case ShaderArguementType::Constant:
			{
				input = new DirectX12ShaderConstant(m_AvailableSlot, size);
				m_Parameters.push_back(input->GetDefinition());
				m_AvailableSlot++;
				break;
			}	
		case ShaderArguementType::Reference:
		{
			input = new DirectX12ShaderReference(m_AvailableSlot, size, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE);
			m_Parameters.push_back(input->GetDefinition());
			m_AvailableSlot++;
			break;
		}
		default:
			GE_CORE_ASSERT(false, "This type of arguement is not yet supported for DX12");
		}

		return input;
	}

	void DirectX12RootSignature::FinalizeSignature() {
		auto pDevice = DirectX12Context::GetDevice();

		// Build root signature description
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc;
		rootSigDesc.Init_1_1((UINT)m_Parameters.size(), m_Parameters.data(), 0 /* Note: Check back on this */, nullptr, m_Flags);

		Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
		bool res = FAILED(D3DX12SerializeVersionedRootSignature(&rootSigDesc, m_RootSigFeat.HighestVersion, &rootSigBlob, &errorBlob)); 
		GE_CORE_ASSERT(!res, "Failed to serialized root signature");

		res = FAILED(pDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_Root)));
		GE_CORE_ASSERT(!res, "Failed to create root signature");
	}

	void DirectX12RootSignature::Append(PipelineStateObject& pipeline) {
		GE_CORE_ASSERT(m_FinalizedSignature, "Need to called FinializeSignature before trying to attach to PipelineStateObject");
		auto& dPipeline = (DirectX12PipelineState&)pipeline;
		dPipeline.Attach(m_Root.Get());
	}

	void DirectX12RootSignature::Bind() {
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->SetGraphicsRootSignature(m_Root.Get());
	}
};

