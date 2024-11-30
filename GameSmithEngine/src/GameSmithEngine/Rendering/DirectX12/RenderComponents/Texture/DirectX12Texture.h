#pragma once
struct D3D12_CPU_DESCRIPTOR_HANDLE;

namespace GameSmith {
	class DirectX12Texture {
	public:
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() = 0;
	};
};
