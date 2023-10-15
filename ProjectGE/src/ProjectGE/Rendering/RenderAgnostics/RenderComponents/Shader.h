#pragma once

namespace ProjectGE {
	// Types of shaders currently supported
	enum Stages {
		STAGE_VERTEX,
		STAGE_HULL,
		STAGE_DOMAIN,
		STAGE_PIXEL,
		STAGE_NUM
	};

	// INTERFACE
	// A piece of code to be ran on the GPU in some part of a render pipline
	class Shader
	{
		virtual Stages GetShaderType() = 0;
	};
};


