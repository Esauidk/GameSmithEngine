#pragma once

#include "GameSmithEngine/Core/Core.h"
#include <gepch.h>

#define GE_REGISTER_PRESET_SHADER(ShaderName, ShaderCode) \
	 static struct ShaderName##RegisterAction { \
		ShaderName##RegisterAction() { \
			GameSmith::ShaderPresetFactory::GetInstance()->RegisterShaderPreset( \
				#ShaderName ".hlsl", \
				ShaderCode \
			); \
		} \
	}ShaderName##Instance;

namespace GameSmith {
	class GE_API ShaderPresetFactory {
	public:
		static ShaderPresetFactory* GetInstance();

		void RegisterShaderPreset(std::string presetName, std::string shaderSource);
		std::string GetShaderSource(std::string presetName);
		bool HasShaderPreset(std::string presetName) const {
			return m_PresetShaders.contains(presetName);
		}
	private:
		static ShaderPresetFactory* s_Instance;
	private:
		std::unordered_map<std::string, std::string> m_PresetShaders;
		friend class ShaderPresetFactory;
	};
};
