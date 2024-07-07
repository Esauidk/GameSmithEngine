#pragma once
#include "ParameterContainer.h"
#include "GameSmithEngine/ResourceAssets/Serializable.h"

namespace GameSmith {
	enum ExposedVariableFlags {
		Asset = 0x1
	};

	struct GE_API ExposedVariableEntry {
		void* originalVariableRef;
		ContainerDataType variableDataType;
		unsigned int flags;
	};


	class GE_API ExposedVariableRegistry : public Serializeable
	{
	public:
		inline void AddExposedVariable(std::string variableName, ExposedVariableEntry entry) { if (entry.originalVariableRef == nullptr) return; m_Registry.insert({ variableName, entry }); }
		void GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap);
		void BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap);


		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		struct RegistrySerializeMetadata {
			unsigned int numVariables;
		};

		std::unordered_map<std::string, ExposedVariableEntry> m_Registry;
	};
};


