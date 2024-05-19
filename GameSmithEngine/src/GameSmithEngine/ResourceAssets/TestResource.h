#pragma once
#include "Serializable.h"

namespace GameSmith {
	class TestResource : public Serializeable
	{
	public:
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	};
};


