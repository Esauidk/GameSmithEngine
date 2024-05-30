#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Utilities/GUIDGenerator.h"


namespace GameSmith {
	struct SerializedMetadata {
		unsigned int id;
		std::string filePath;
	};

	// Intertface for a class that can be serialize/deserialized
	class Serializeable {
	public:
		// TODO: Get a cross-platform way to generate GUIDs
		Serializeable() : m_ID(GUIDGenerator::GenerateID()){}

		// Serialize the interface implementor
		// Returns a pointer to a byte array containing the serialized class
		virtual Ref<char> Serialize() = 0;

		// Serializes the interface implementor
		// Appends the serialization of the implementor to the byte stream
		virtual void Serialize(char* byteStream, unsigned int availableBytes) = 0;

		// Returns the amount of bytes required to serialize the interface implementor
		virtual unsigned int RequireSpace() const = 0;

		// Deserializes a byte array into the given class
		virtual void Deserialize(char* inData, unsigned int size) = 0;

		inline const ID& GetId() { return m_ID; }
		inline void SetId(ID& newId) { m_ID = newId; }
	private:
		ID m_ID;
	};
};
