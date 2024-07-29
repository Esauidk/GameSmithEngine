#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Utilities/GUIDGenerator.h"

#define SERIAL_FILE(FILE_TYPE, FILE_EXT) static std::string GetStaticFileExtension() {return #FILE_EXT;} \
										static std::string GetStaticFileType() {return #FILE_TYPE;} \
										virtual std::string GetFileExtension() const override {return GetStaticFileExtension();} \
										virtual std::string GetFileType() const override {return GetStaticFileType();}

namespace GameSmith {
	// Intertface for a class that can be serialize/deserialized
	class GE_API Serializeable {
	public:
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

		virtual std::string GetFileExtension() const { return "*"; }
		virtual std::string GetFileType() const { return "Any"; }

		inline const ID& GetId() { return m_ID; }
		inline void SetId(ID& newId) { m_ID = newId; }
	private:
		ID m_ID;
	};
};
