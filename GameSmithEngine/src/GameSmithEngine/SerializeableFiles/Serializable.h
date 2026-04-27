#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Utilities/GUIDGenerator.h"

namespace GameSmith {
	// Intertface for a class that can be serialize/deserialized
	class GE_API ISerializeable : public virtual IDObjectInterface {
	public:
		virtual ~ISerializeable() = default;

		// Serialize the interface implementor
		// Returns a pointer to a byte array containing the serialized class
		virtual Ref<char> Serialize() = 0;

		// Serializes the interface implementor
		// Appends the serialization of the implementor to the byte stream
		virtual void Serialize(char* byteStream, unsigned int availableBytes) = 0;

		// Returns the amount of bytes required to serialize the interface implementor
		virtual unsigned int RequiredSpace() const = 0;

		// Deserializes a byte array into the given class
		virtual void Deserialize(char* inData, unsigned int size) = 0;
	};

	// An abstract base class for serializeable files
	class GE_API AbstractBaseSerializeable : public IDObject, public virtual ISerializeable {
	};
};
