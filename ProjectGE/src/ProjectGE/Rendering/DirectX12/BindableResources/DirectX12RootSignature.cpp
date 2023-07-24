#include "gepch.h"
#include "DirectX12RootSignature.h"
#include "ProjectGE/Log.h"


namespace ProjectGE {
	DirectX12RootSignature::DirectX12RootSignature(ID3D12Device8* pDevice, D3D12_ROOT_SIGNATURE_FLAGS flags) : m_RootSigFeat(), m_Flags(flags) {
		m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &m_RootSigFeat, sizeof(m_RootSigFeat)))) {
			m_RootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
	}

	void DirectX12RootSignature::AddParameter(const CD3DX12_ROOT_PARAMETER1& newParameter) {
		m_Parameters.push_back(newParameter);
	}

	void DirectX12RootSignature::AddParameter(CD3DX12_ROOT_PARAMETER1* newParameters, UINT size) {
		m_Parameters.insert(m_Parameters.end(), newParameters, newParameters + size);
	}

	void DirectX12RootSignature::BuildRootSignature(ID3D12Device8* pDevice) {
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

	void DirectX12RootSignature::Setup(DirectX12PipelineState& pipeline) {
		pipeline.Attach(m_Root.Get());
	}

	void DirectX12RootSignature::Bind(ID3D12GraphicsCommandList6* cmdList) {
		cmdList->SetGraphicsRootSignature(m_Root.Get());
	}
};

