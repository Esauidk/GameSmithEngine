#include "gepch.h"
#include "HeapResourceLoader.h"
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	char* ProjectGE::HeapResourceLoader::LoadResource(std::string file, size_t* outSize)
	{
		std::fstream pFile(file, std::ios::in);

		GE_CORE_ASSERT(!pFile.is_open(), std::format("Asset file {} cannot be opened", file));

		pFile.seekg(0, std::ios::end);
		*outSize = pFile.tellg();
		pFile.seekg(0, std::ios::beg);

		char* buf = new char[*outSize];

		pFile.read(buf, *outSize);
		pFile.close();

		return buf;
	}
};

