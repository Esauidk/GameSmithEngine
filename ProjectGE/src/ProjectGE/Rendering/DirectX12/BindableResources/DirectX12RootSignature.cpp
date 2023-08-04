#include "gepch.h"
#include "DirectX12RootSignature.h"
#include "ProjectGE/Log.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12ShaderConstant.h"


namespace ProjectGE {
	DirectX12RootSignature::DirectX12RootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags) : m_RootSigFeat(), m_Flags(flags) {
		auto pDevice = DirectX12Context::GetDevice();
		m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &m_RootSigFeat, sizeof(m_RootSigFeat)))) {
			m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
	}

	ShaderArguement* DirectX12RootSignature::AddArguement(void* data, UINT size, ShaderArguementType type)
	{
		switch (type) {
		case ShaderArguementType::Constant:
			{
				auto* constant = new DirectX12ShaderConstant(m_AvailableSlot, data, size);
				m_Parameters.push_back(constant->GetDefinition());
				m_AvailableSlot++;
				return constant;
			}	
		default:
			GE_CORE_ASSERT(false, "This type of arguement is not yet supported for DX12");
		}

		return nullptr;
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

