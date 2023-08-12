#pragma once
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"

namespace ProjectGE {
	// Types of shaders currently supported
	enum class ShaderType {
		None = 0,
		Vertex,
		Pixel
	};

	// INTERFACE
	// A piece of code to be ran on the GPU in some part of a render pipline
	class Shader : public BindableResource, public PipelineDefiner
	{
	public:
		// Instantiates an implementation of the Shader Interface (recommended to use this instead of instantiating a specific implementation)
		static Shader* Create(std::string vertexPath, std::string pixelPath);
		static Shader* Create(std::string vertexPath, std::string pixelPath, ShaderArguement* shaderUnifromsSlot, UINT size);
	};
};


