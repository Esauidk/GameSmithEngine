#pragma once
#include "Asset.h"

namespace GameSmith {
	class TestResource : public Asset
	{
	public:
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	};
};


