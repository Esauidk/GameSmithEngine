#pragma once

namespace GameSmith {
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
	public:
		virtual Stages GetShaderType() = 0;
		virtual unsigned int GetHash() = 0;
	};
};


