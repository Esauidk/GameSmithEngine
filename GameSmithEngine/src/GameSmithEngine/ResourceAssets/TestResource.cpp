#include "gepch.h"
#include "TestResource.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	Ref<char> TestResource::Serialize()
	{
		GE_CORE_INFO("Wrote Resource resource!");
		return Ref<char>();
	}

	void TestResource::Serialize(char* byteStream, unsigned int availableBytes)
	{
		GE_CORE_INFO("Wrote Resource resource!");
	}

	unsigned int TestResource::RequiredSpace() const
	{
		return 0;
	}

	void TestResource::Deserialize(char* inData, unsigned int size)
	{
		GE_CORE_INFO("Loaded resource!");
	}
};

