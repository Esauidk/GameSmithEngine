#pragma once
#include <wrl.h>
#include <d3d12.h>

// This class is used as an interface for all resources that are related to the graphics pipeline
namespace Render {
	class BindableResource {
	public:
		// Binding the resource to the pipeline
		virtual void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) = 0;
		virtual ~BindableResource() = default;
	};
};

