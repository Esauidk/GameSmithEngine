#include "gepch.h"
#include "ExposedVariableRegistry.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	void ExposedVariableRegistry::GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap)
	{
		for (auto& entry : m_Registry) {
			auto container = ConvertToParameter(entry.first, entry.second.variableDataType, (char*)entry.second.originalVariableRef);
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

	Ref<char> ExposedVariableRegistry::Serialize()
	{
		ResourceAssetWriter writer(RequireSpace());
		
		RegistrySerializeMetadata meta;
		meta.numVariables = (unsigned int)m_Registry.size();

		writer.WriteClass<RegistrySerializeMetadata>(&meta);

		for (auto& entry : m_Registry) {
			writer.WriteString(entry.first);
			writer.WriteClass<ContainerDataType>(&(entry.second.variableDataType));
			writer.WriteByte((char*)entry.second.originalVariableRef, GetParameterSize(entry.second.variableDataType));
		}

		return Ref<char>(writer.GetBuffer());
	}

	void ExposedVariableRegistry::Serialize(char* byteStream, unsigned int availableBytes)
	{
		GE_CORE_ASSERT(availableBytes >= RequireSpace(), "There's not enough space to serialize the ExposedVariableRegistry");

		ResourceAssetWriter writer(byteStream, availableBytes);

		RegistrySerializeMetadata meta;
		meta.numVariables = (unsigned int)m_Registry.size();

		writer.WriteClass<RegistrySerializeMetadata>(&meta);

		for (auto& entry : m_Registry) {
			writer.WriteString(entry.first);
			writer.WriteClass<ContainerDataType>(&(entry.second.variableDataType));
			writer.WriteByte((char*)entry.second.originalVariableRef, GetParameterSize(entry.second.variableDataType));
		}

	}

	unsigned int ExposedVariableRegistry::RequireSpace() const
	{
		unsigned int size = 0;
		size += sizeof(RegistrySerializeMetadata);

		for (auto& entry : m_Registry) {
			size += (unsigned int)(entry.first.length() + 1);
			size += sizeof(ContainerDataType);
			size += GetParameterSize(entry.second.variableDataType);
		}

		return size;
	}

	void ExposedVariableRegistry::Deserialize(char* inData, unsigned int size)
	{
		ResourceAssetReader reader(inData, size);
		RegistrySerializeMetadata* meta = reader.ReadClass<RegistrySerializeMetadata>();

		int i = 0;
		while (i < meta->numVariables) {
			std::string name = reader.GetString();
			ContainerDataType* dataType = reader.ReadClass<ContainerDataType>();
			unsigned int paramSize = GetParameterSize(*dataType);
			if (m_Registry.contains(name)) {
				auto entry = m_Registry.find(name);
				memcpy(entry->second.originalVariableRef, reader.GetCurPtr(), paramSize);
			}

			reader.MoveForward(paramSize);
			i++;
		}
		
	}
};
