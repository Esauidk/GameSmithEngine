#pragma once
#include "ProjectGE/Rendering/BindableResource.h"

namespace ProjectGE {
	// Types of Shader Arguements Supported
	enum class ShaderArguementType {
		None = 0,
		Constant,
		ConstantBuffer,
		ShaderResource,
		ConstantBufferTable,
		ShaderResourceTable
	};

	// INTERFACE
	// A wrapper to represent an arguement of a shader
	class ShaderArguement : public BindableResource
	{
	public:
		// Set the data of the shader arguement
		virtual void SetData(void* rawData) = 0;
	};
};


