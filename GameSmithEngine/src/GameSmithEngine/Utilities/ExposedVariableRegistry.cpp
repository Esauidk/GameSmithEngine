#include "gepch.h"
#include "ExposedVariableRegistry.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"

namespace GameSmith {
	void ExposedVariableRegistry::AddExposedVariable(const std::string variableName, Ref<ParameterContainer> container, const std::string& groupName)
	{
		m_ValueRegistry.insert({ variableName, {nullptr, 0, container->GetType(), groupName, container->GetFlags(), container, true} });
		AddToGroup(groupName, variableName);
	}

	void ExposedVariableRegistry::RemoveExposedVariable(const std::string variableName)
	{
		if (m_ValueRegistry.contains(variableName)) {
			std::string groupName = m_ValueRegistry[variableName].groupName;
			m_ValueRegistry.erase(variableName);
			RemoveFromGroup(groupName, variableName);
		}
	}

	void ExposedVariableRegistry::RemoveExposedConnection(const std::string refName)
	{
		if (m_ConnectionsRegistry.contains(refName)) {
			std::string groupName = m_ConnectionsRegistry[refName].groupName;
			m_ConnectionsRegistry.erase(refName);
			RemoveFromGroup(groupName, refName);
		}
	}

	void ExposedVariableRegistry::RemoveExposedAsset(const std::string refName)
	{
		if (m_AssetRegistry.contains(refName)) {
			std::string groupName = m_AssetRegistry[refName].groupName;
			m_AssetRegistry.erase(refName);
			RemoveFromGroup(groupName, refName);
		}
	}

	void ExposedVariableRegistry::GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap)
	{
		for (auto& entry : m_ValueRegistry) {
			Ref<ParameterContainer> container;

			if (entry.second.ownsContainer) {
				container = entry.second.containerRef->MakeCopy();
			}
			else {
				container = ConvertToParameter(entry.first, entry.second.variableDataType, (char*)entry.second.originalVariableRef, entry.second.variableSize);
			}
			outMap->insert({ container->GetName(), container });
		}
	}

	void ExposedVariableRegistry::GenerateConnectionsMap(std::unordered_map<std::string, Ref<ConnectionContainer>>* outMap)
	{
		for (auto& entry : m_ConnectionsRegistry) {
			Ref<ConnectionContainer> container = Ref<ConnectionContainer>(new ObjectRefContainer(entry.second.conversionFunction, entry.second.typeName, entry.second.flag));
			container->AssignRef(*(entry.second.originalRefRef));
			container->AssignID(entry.second.objectID);

			outMap->insert({ entry.first, container });
		}
	}

	void ExposedVariableRegistry::GenerateAssetMap(std::unordered_map<std::string, Ref<AssetRefContainer>>* outMap)
	{
		for (auto& entry : m_AssetRegistry) {
			Ref<AssetRefContainer> container = Ref<AssetRefContainer>(new ObjectRefContainer(entry.second.conversionFunction, entry.second.typeName, entry.second.flag));
			container->AssignRef(*(entry.second.originalRefRef));
			container->AssignID(entry.second.objectID);

			outMap->insert({ entry.first, container });
		}
	}

	void ExposedVariableRegistry::BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap)
	{
		for (auto& entry : m_ValueRegistry) {
			if (inMap.contains(entry.first)) {
				const auto& variable = inMap.find(entry.first);
				if (!entry.second.ownsContainer) {
					memcpy(entry.second.originalVariableRef, variable->second->GetCharData(), variable->second->GetSize());
				}
				else {
					memcpy(entry.second.containerRef->GetCharData(), variable->second->GetCharData(), variable->second->GetSize());
				}

			}
		}
	}

	void ExposedVariableRegistry::BootstrapFromConnectionsMap(const std::unordered_map<std::string, Ref<ConnectionContainer>>& inMap)
	{
		for (auto& entry : m_ConnectionsRegistry) {
			if (inMap.contains(entry.first)) {
				const auto& ref = inMap.find(entry.first);
				entry.second.assignmentFunction(entry.second.originalRefRef, ref->second->GetCurrentRef());
				entry.second.objectID = ref->second->GetCurrentRefID();
				entry.second.flag = ref->second->GetFlags();;
			}
		}
	}

	void ExposedVariableRegistry::BootstrapFromAssetMap(const std::unordered_map<std::string, Ref<AssetRefContainer>>& inMap)
	{
		for (auto& entry : m_AssetRegistry) {
			if (inMap.contains(entry.first)) {
				const auto& ref = inMap.find(entry.first);
				entry.second.assignmentFunction(entry.second.originalRefRef, ref->second->GetCurrentRef());
				entry.second.objectID = ref->second->GetCurrentRefID();
				entry.second.flag = ref->second->GetFlags();
			}
		}
	}

	Ref<char> ExposedVariableRegistry::Serialize()
	{
		BinaryStreamWriter writer(RequiredSpace());

		Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());

		return Ref<char>(writer.GetBuffer());
	}

	void ExposedVariableRegistry::Serialize(char* byteStream, unsigned int availableBytes)
	{
		GE_CORE_ASSERT(availableBytes >= RequiredSpace(), "There's not enough space to serialize the ExposedVariableRegistry");

		BinaryStreamWriter writer(byteStream, availableBytes);

		RegistrySerializeMetadata meta;
		meta.numVariables = (unsigned int)m_ValueRegistry.size();
		meta.numConnections = (unsigned int)m_ConnectionsRegistry.size();
		meta.numAssetRefs = (unsigned int)m_AssetRegistry.size();

		writer.WriteClass<RegistrySerializeMetadata>(&meta);

		for (auto& entry : m_ValueRegistry) {
			writer.WriteString(entry.first);
			writer.WriteClass<ContainerDataType>(&(entry.second.variableDataType));
			writer.WriteByte((char*)entry.second.originalVariableRef, GetParameterSize(entry.second.variableDataType));
		}

		for (auto& entry : m_ConnectionsRegistry) {
			writer.WriteString(entry.first);
			idData rawId = entry.second.objectID.getData();
			writer.WriteClass<idData>(&rawId);
			writer.WriteUInt(entry.second.flag);
		}

		for (auto& entry : m_AssetRegistry) {
			writer.WriteString(entry.first);
			idData rawId = entry.second.objectID.getData();
			writer.WriteClass<idData>(&rawId);
			writer.WriteUInt(entry.second.flag);
		}
	}

	unsigned int ExposedVariableRegistry::RequiredSpace() const
	{
		unsigned int size = 0;
		size += sizeof(RegistrySerializeMetadata);

		for (auto& entry : m_ValueRegistry) {
			size += (unsigned int)(entry.first.length() + 1);
			size += sizeof(ContainerDataType);
			size += GetParameterSize(entry.second.variableDataType);
		}

		for (auto& entry : m_ConnectionsRegistry) {
			size += (unsigned int)(entry.first.length() + 1);
			size += sizeof(idData);
			size += sizeof(unsigned int);
		}

		for (auto& entry : m_AssetRegistry) {
			size += (unsigned int)(entry.first.length() + 1);
			size += sizeof(idData);
			size += sizeof(unsigned int);
		}

		return size;
	}

	void ExposedVariableRegistry::Deserialize(char* inData, unsigned int size)
	{
		BinaryStreamReader reader(inData, size);
		RegistrySerializeMetadata* meta = reader.ReadClass<RegistrySerializeMetadata>();

		unsigned int i = 0;
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

		// Only the IDs are saved, the original pointers will need to be updated by the caller after deserialization using the saved IDs
		i = 0;
		while (i < meta->numConnections) {
			std::string name = reader.GetString();
			idData* rawID = reader.ReadClass<idData>();
			unsigned int flags = reader.GetUInt();
			if (m_ConnectionsRegistry.contains(name)) {
				auto entry = m_ConnectionsRegistry.find(name);
				entry->second.objectID = ID(*rawID);
				entry->second.flag = flags;
			}

			i++;
		}

		i = 0;
		while (i < meta->numAssetRefs) {
			std::string name = reader.GetString();
			idData* rawID = reader.ReadClass<idData>();
			unsigned int flags = reader.GetUInt();
			if (m_AssetRegistry.contains(name)) {
				auto entry = m_AssetRegistry.find(name);
				entry->second.objectID = ID(*rawID);
				entry->second.flag = flags;
			}

			i++;
		}
	}

	void ExposedVariableRegistry::AddToGroup(const std::string& groupName, const std::string& variableName)
	{
		if (m_GroupingRegistry.contains(groupName)) {
			m_GroupingRegistry[groupName].insert(variableName);
		}
		else {
			m_GroupingRegistry[groupName] = { variableName };
		}
	}

	void ExposedVariableRegistry::RemoveFromGroup(const std::string& groupName, const std::string& variableName)
	{
		if (m_GroupingRegistry.contains(groupName)) {
			auto& group = m_GroupingRegistry[groupName];
			group.erase(variableName);
			if (group.empty()) {
				m_GroupingRegistry.erase(groupName);
			}
		}
	}
};