#include "gepch.h"
#include "DirectX12Sampler.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

namespace ProjectGE {

	static D3D12_FILTER TranslateFilterType(FilterType img) {
		switch (img) {
		case FilterType::Point:
			return D3D12_FILTER_MIN_MAG_MIP_POINT;
		case FilterType::Linear:
			return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		case FilterType::Anisotropic:
			return D3D12_FILTER_ANISOTROPIC;
		}

		return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	}

	static D3D12_TEXTURE_ADDRESS_MODE TranslatePaddingMethod(PaddingMethod padMode) {
		switch (padMode) {
		case PaddingMethod::Wrap:
			return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		case PaddingMethod::Clamp:
			return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		case PaddingMethod::Mirror:
			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
		}

		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	}

	DirectX12Sampler::DirectX12Sampler(FilterType img, PaddingMethod padMode)
	{
		D3D12_SAMPLER_DESC desc;
		desc.Filter = TranslateFilterType(img);

		D3D12_TEXTURE_ADDRESS_MODE padding = TranslatePaddingMethod(padMode);
		desc.AddressU = padding;
		desc.AddressV = padding;
		desc.AddressW = padding;
		desc.MipLODBias = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = 0;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;

		m_TempDescriptor = DirectX12Core::GetCore().GetDescriptorLoader(SAMPLER).AllocateSlot();

		GenerateSampler(desc);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Sampler::GetDescriptor()
	{
		return m_TempDescriptor;
	}

	void DirectX12Sampler::GenerateSampler(D3D12_SAMPLER_DESC& desc)
	{
		DirectX12Core::GetCore().GetDevice()->CreateSampler(&desc, m_TempDescriptor);
	}
};