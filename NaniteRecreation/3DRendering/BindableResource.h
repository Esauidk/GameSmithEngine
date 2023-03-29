#pragma once

// This class is used as an interface for all resources that are related to the graphics pipeline
namespace Render {
	class BindableResource {
	public:
		// Binding the resource to the pipeline
		virtual void Bind() = 0;
		virtual ~BindableResource() = default;
	};
};

