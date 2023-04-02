#include "RootSignature.h"
#include "DirectXMacros.h"


namespace Render {
	RootSignature::RootSignature(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, D3D12_ROOT_SIGNATURE_FLAGS flags) : rootSigFeat({}), flags(flags) {
		rootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &rootSigFeat, sizeof(rootSigFeat)))) {
			rootSigFeat.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
	}

	void RootSignature::AddParameter(CD3DX12_ROOT_PARAMETER1 newParameter) {
		parameters.push_back(newParameter);
	}

	void RootSignature::AddParameter(CD3DX12_ROOT_PARAMETER1* newParameters, UINT size) {
		parameters.insert(parameters.end(), newParameters, newParameters + size);
	}

	void RootSignature::BuildRootSignature(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice) {
		// Build root signature description
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc;
		rootSigDesc.Init_1_1(parameters.size(), parameters.data(), 0 /* Note: Check back on this */, nullptr, flags);

		Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
		RENDER_THROW(D3DX12SerializeVersionedRootSignature(&rootSigDesc, rootSigFeat.HighestVersion, &rootSigBlob, &errorBlob)); \

		RENDER_THROW(pDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSig)));
	}

	void RootSignature::Bind() {
		// Implementing Binding
		throw std::exception("Ran");
		return;
	}
};

