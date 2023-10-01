#pragma once
#include "ProjectGE/ResourceManagement/ResourceLoaders/ResourceLoader.h"

namespace ProjectGE {
	class HeapResourceLoader : public ResourceLoader {
	public:
		virtual char* LoadResource(std::string file, size_t* outSize) override;
	};
};

