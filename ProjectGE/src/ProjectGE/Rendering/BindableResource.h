#pragma once
#include <wrl.h>
#include <d3d12.h>

// INTERFACE
// This class is used as an interface for all resources that are related to the graphics pipeline
namespace ProjectGE {
	class BindableResource {
	public:
		// Binding the resource to the pipeline
		virtual void Bind() = 0;
	};
};

