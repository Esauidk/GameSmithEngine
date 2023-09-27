#pragma once

namespace ProjectGE {
	// Types of shaders currently supported
	enum ShaderType {
		Vertex,
		Pixel,
		ShaderCount
	};

	// INTERFACE
	// A piece of code to be ran on the GPU in some part of a render pipline
	class Shader
	{
		virtual ShaderType GetShaderType() = 0;
	};
};


