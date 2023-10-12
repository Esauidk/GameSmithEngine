#include "gepch.h"
#include "HeapResourceLoader.h"
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	char* ProjectGE::HeapResourceLoader::LoadResource(std::string file, UINT* outSize)
	{
		std::fstream pFile(file, std::ios::in | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", file));

		*outSize = (UINT)pFile.tellg();
		pFile.seekg(0, pFile.beg);

		char* buf = new char[*outSize];

		pFile.read(buf, *outSize);
		pFile.close();

		return buf;
	}

	char* HeapResourceLoader::LoadResource(char* data, UINT size)
	{
		char* buf = new char[size];
		memcpy(buf, data, size);

		return buf;
	}

	void HeapResourceLoader::CleanResource(char* resource)
	{
		delete[] resource;
	}
};

