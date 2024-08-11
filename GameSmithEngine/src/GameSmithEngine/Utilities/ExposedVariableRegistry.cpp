#include "gepch.h"
#include "ExposedVariableRegistry.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	void ExposedVariableRegistry::GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap)
	{
		for (auto& entry : m_ValueRegistry) {
			auto container = ConvertToParameter(entry.first, entry.second.variableDataType, (char*)entry.second.originalVariableRef);
			outMap->insert({ container->GetName(), container });
		}
	}

	void ExposedVariableRegistry::GenerateReferenceMap(std::unordered_map<std::string, Ref<RefContainer>>* outMap)
	{
		for (auto& entry : m_RefRegistry) {
			Ref<RefContainer> container = Ref<RefContainer>(new RefContainer(entry.second.conversionFunction, "test"));
			container->AssignRef(*(entry.second.originalRefRef), entry.second.flag);
			container->AssignID(entry.second.objectID);

			outMap->insert({ entry.first, container });
		}
	}

	void ExposedVariableRegistry::BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap)
	{
		for (auto& entry : m_ValueRegistry) {
			if (inMap.contains(entry.first)) {
				const auto& variable = inMap.find(entry.first);
				memcpy(entry.second.originalVariableRef, variable->second->GetCharData(), variable->second->GetSize());
			}
		}
	}

	void ExposedVariableRegistry::BootstrapFromRefMap(const std::unordered_map<std::string, Ref<RefContainer>>& inMap)
	{
		for (auto& entry : m_RefRegistry) {
			if (inMap.contains(entry.first)) {
				const auto& ref = inMap.find(entry.first);
				entry.second.assignmentFunction(entry.second.originalRefRef, ref->second->GetCurrentRef());
				entry.second.objectID = ref->second->GetCurrentRefID();
				entry.second.flag = ref->second->GetFlags();;
			}
		}
	}

	Ref<char> ExposedVariableRegistry::Serialize()
	{
		ResourceAssetWriter writer(RequireSpace());
		
		RegistrySerializeMetadata meta;
		meta.numVariables = (unsigned int)m_ValueRegistry.size();
		meta.numRefs = (unsigned int)m_RefRegistry.size();

		writer.WriteClass<RegistrySerializeMetadata>(&meta);

		for (auto& entry : m_ValueRegistry) {
			writer.WriteString(entry.first);
			writer.WriteClass<ContainerDataType>(&(entry.second.variableDataType));
			writer.WriteByte((char*)entry.second.originalVariableRef, GetParameterSize(entry.second.variableDataType));
		}

		for (auto& entry : m_RefRegistry) {
			writer.WriteString(entry.first);
			idData rawId = entry.second.objectID.getData();
			writer.WriteClass<idData>(&rawId);
			writer.WriteUInt(entry.second.flag);
		}

		return Ref<char>(writer.GetBuffer());
	}

	void ExposedVariableRegistry::Serialize(char* byteStream, unsigned int availableBytes)
	{
		GE_CORE_ASSERT(availableBytes >= RequireSpace(), "There's not enough space to serialize the ExposedVariableRegistry");

		ResourceAssetWriter writer(byteStream, availableBytes);

		RegistrySerializeMetadata meta;
		meta.numVariables = (unsigned int)m_ValueRegistry.size();
		meta.numRefs = (unsigned int)m_RefRegistry.size();

		writer.WriteClass<RegistrySerializeMetadata>(&meta);

		for (auto& entry : m_ValueRegistry) {
			writer.WriteString(entry.first);
			writer.WriteClass<ContainerDataType>(&(entry.second.variableDataType));
			writer.WriteByte((char*)entry.second.originalVariableRef, GetParameterSize(entry.second.variableDataType));
		}

		for (auto& entry : m_RefRegistry) {
			writer.WriteString(entry.first);
			idData rawId = entry.second.objectID.getData();
			writer.WriteClass<idData>(&rawId);
			writer.WriteUInt(entry.second.flag);
		}

	}

	unsigned int ExposedVariableRegistry::RequireSpace() const
	{
		unsigned int size = 0;
		size += sizeof(RegistrySerializeMetadata);

		for (auto& entry : m_ValueRegistry) {
			size += (unsigned int)(entry.first.length() + 1);
			size += sizeof(ContainerDataType);
			size += GetParameterSize(entry.second.variableDataType);
		}

		for (auto& entry : m_RefRegistry) {
			size += (unsigned int)(entry.first.length() + 1);
			size += sizeof(idData);
			size += sizeof(unsigned int);
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
			if (m_ValueRegistry.contains(name)) {
				auto entry = m_ValueRegistry.find(name);
				memcpy(entry->second.originalVariableRef, reader.GetCurPtr(), paramSize);
			}

			reader.MoveForward(paramSize);
			i++;
		}
		

		i = 0;
		while (i < meta->numRefs) {
			std::string name = reader.GetString();
			idData* rawID = reader.ReadClass<idData>();
			unsigned int flags = reader.GetUInt();
			if (m_RefRegistry.contains(name)) {
				auto entry = m_RefRegistry.find(name);
				entry->second.objectID = ID(*rawID);
				entry->second.flag = flags;
			}

			i++;
		}
	}

	void RefContainer::AssignRef(Connection<IDObject> toAssign, unsigned int flags)
	{
		if (!TypeCheck(toAssign)) {
			return;
		}

		m_CopyRef = toAssign;
		m_Flag = flags;
	}
};
