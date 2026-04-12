#pragma once
#include "../Asset.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace GameSmith {
	class ShaderConfiguration : public Asset {
	public:
		ShaderConfiguration(std::string name) : Asset(name) {};

		SERIAL_FILE(ShaderConfig, GMsrcfg)
	private:
	};
};
