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
	class DirectX12RootSignature
	{
	public: 
		DirectX12RootSignature() = default;
		void Init(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc);
		void InitGenericRootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags);
		inline ID3D12RootSignature* GetInternalRootSignature() { return m_Root.Get(); }
			//void Bind() override;
	private:
		void IndexRootSignature(D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc);
		D3D12_FEATURE_DATA_ROOT_SIGNATURE m_RootSigFeat;
		ComPtr<ID3D12RootSignature> m_Root;
		UINT m_AvailableSlot;
		Microsoft::WRL::ComPtr<ID3DBlob> m_RootSigBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ErrorBlob;
		UINT RegisterSlotTable[];
	};
}


