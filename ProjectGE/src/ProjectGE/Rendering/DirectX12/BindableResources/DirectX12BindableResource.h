#pragma once
#include <wrl.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;
// This class is used as an interface for all resources that are related to the graphics pipeline
namespace ProjectGE {
	class DirectX12BindableResource {
	public:
		// Binding the resource to the pipeline, does not take ownership of the command list pointer
		virtual void Bind(ID3D12GraphicsCommandList6* cmdList) {/* To allow super classes to not implent bind if there is no extra behavior */ };
		virtual ~DirectX12BindableResource() = default;
	};
};

