#pragma once
#include "ProjectGE/ResourceManagement/ResourceLoaders/ResourceLoader.h"

namespace ProjectGE {
	class HeapResourceLoader : public ResourceLoader {
	public:
		virtual char* LoadResource(std::string file, UINT* outSize) override;
		virtual void CleanResource(char* resource) override;
	};
};

