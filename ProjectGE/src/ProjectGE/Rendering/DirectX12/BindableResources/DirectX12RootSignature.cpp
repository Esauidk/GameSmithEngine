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
	}

	void DirectX12RootSignature::InitGenericRootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags)
	{
		DirectX12RootSignatureBuilder builder;
		CreateLargeGraphicsRootSignature(builder, flags);
		Init(builder.Build());
	}

	/*void DirectX12RootSignature::Bind() {
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->SetGraphicsRootSignature(m_Root.Get());
	}*/
};

