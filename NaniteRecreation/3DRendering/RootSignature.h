#pragma once
#include "BindableResource.h"
#include <d3d12.h>
#include <wrl.h>

namespace Render {
	class RootSignature : public BindableResource
	{
	public: 
			RootSignature(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, D3D12_ROOT_SIGNATURE_FLAGS flags);
			void Bind() override;
	private:
		D3D12_FEATURE_DATA_ROOT_SIGNATURE rootSig;
	};
}


