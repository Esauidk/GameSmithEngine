#pragma once
#include "ParameterContainer.h"
#include "GameSmithEngine/ResourceAssets/Serializable.h"

namespace GameSmith {
	enum ExposedVariableFlags {
		Asset = 0x1
	};

	struct ExposedVariableEntry {
		void* originalVariableRef;
		ContainerDataType variableDataType;
		unsigned int flags;
	};


	class ExposedVariableRegistry : public Serializeable
	{
	public:
		inline void AddExposedVariable(std::string variableName, ExposedVariableEntry entry) { if (entry.originalVariableRef == nullptr) return; m_Registry.insert({ variableName, entry }); }
		void GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap);
		void BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap);

		// Serialize the interface implementor
		// Returns a pointer to a byte array containing the serialized class
		virtual Ref<char> Serialize() override;

		// Serializes the interface implementor
		// Appends the serialization of the implementor to the byte stream
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;

		// Returns the amount of bytes required to serialize the interface implementor
		virtual unsigned int RequireSpace() const override;

		// Deserializes a byte array into the given class
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		struct RegistrySerializeMetadata {
			unsigned int numVariables;
		};

		std::unordered_map<std::string, ExposedVariableEntry> m_Registry;
	};
};


