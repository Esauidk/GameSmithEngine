#pragma once
#include "ProjectGE/Rendering/BindableResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"

namespace ProjectGE {
	enum class ShaderType {
		None = 0,
		Vertex,
		Pixel
	};

	class Shader : public BindableResource, public PipelineDefiner
	{
	public:
		static Shader* Create(std::string path, ShaderType count);
	};
};


