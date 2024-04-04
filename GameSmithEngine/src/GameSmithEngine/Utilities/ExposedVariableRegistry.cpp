#include "gepch.h"
#include "ExposedVariableRegistry.h"

namespace GameSmith {
	void ExposedVariableRegistry::GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap)
	{
		for (auto& entry : m_Registry) {
			auto container = CreateContainer(entry.first, entry.second.variableDataType);
			outMap->insert({ container->GetName(), container });
		}
	}

	void ExposedVariableRegistry::BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap)
	{
		for (auto& entry : m_Registry) {
			if (inMap.contains(entry.first)) {
				const auto& variable = inMap.find(entry.first);
				memcpy(entry.second.originalVariableRef, variable->second->GetCharData(), variable->second->GetSize());
			}
		}
	}
};
