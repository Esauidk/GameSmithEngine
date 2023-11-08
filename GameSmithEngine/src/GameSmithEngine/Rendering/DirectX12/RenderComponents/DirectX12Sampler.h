#pragma once
#include <d3d12.h>
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12DescriptorLoaderHeapManager.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Sampler.h"

namespace GameSmith {
	class DirectX12Sampler : public Sampler
	{
	public:
		DirectX12Sampler(FilterType img, PaddingMethod padMode);
		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor();

		inline virtual FilterType GetFilterType() const override { return m_Filter; };
		inline virtual PaddingMethod GetPaddingMethod() const override { return m_Padding; };
	private:
		void GenerateSampler(D3D12_SAMPLER_DESC& desc);
	private:
		FilterType m_Filter;
		PaddingMethod m_Padding;
		DirectX12LoaderDescriptor m_TempDescriptor;
	};
};


