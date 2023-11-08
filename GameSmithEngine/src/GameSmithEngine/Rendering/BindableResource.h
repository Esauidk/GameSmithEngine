#pragma once

// INTERFACE
// This class is used as an interface for all resources that are related to the graphics pipeline
namespace GameSmith {
	class BindableResource {
	public:
		virtual ~BindableResource() = default;
		// Binding the resource to the pipeline
		virtual void Bind() = 0;
	};
};

