#include <gepch.h>
#include "ShaderPresetsFactory.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	ShaderPresetFactory* ShaderPresetFactory::s_Instance = nullptr;

	ShaderPresetFactory* ShaderPresetFactory::GetInstance()
	{
		if (s_Instance == nullptr) {
			s_Instance = new ShaderPresetFactory();
		}

		return s_Instance;
	}

	void ShaderPresetFactory::RegisterShaderPreset(std::string presetName, std::string shaderSource)
	{
		GE_CORE_ASSERT(!m_PresetShaders.contains(presetName), std::format("Shader preset with name {0} already exists!", presetName));
		m_PresetShaders[presetName] = shaderSource;
	}

	std::string ShaderPresetFactory::GetShaderSource(std::string presetName)
	{
		if (!m_PresetShaders.contains(presetName)) {
			return "";
		}
		return m_PresetShaders[presetName];
	}
};