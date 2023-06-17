#pragma once
#include "PipelineDefiner.h"
#include "../../../Util/DirectX12/d3dx12.h"
#include <vector>

/* 
This class is a encapulated version of the Root Signature (An object linking paramters to stages in the render pipeline)
Job:
	- Collects flags and parameters
	- Serializes and Build the Root Signature
	- Binds it to the pipeline
*/
namespace Render {
	class RootSignature : public PipelineDefiner
	{
	public: 
			RootSignature(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, D3D12_ROOT_SIGNATURE_FLAGS flags);
			void AddParameter(CD3DX12_ROOT_PARAMETER1 newParameter);
			void AddParameter(CD3DX12_ROOT_PARAMETER1* newParameters, UINT size);
			void BuildRootSignature(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice);
			void Setup(PipelineState& pipeline) override;
			void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	private:
		D3D12_FEATURE_DATA_ROOT_SIGNATURE rootSigFeat;
		D3D12_ROOT_SIGNATURE_FLAGS flags;
		std::vector<CD3DX12_ROOT_PARAMETER1> parameters;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> pRoot;
	};
}


