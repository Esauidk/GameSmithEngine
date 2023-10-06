#pragma once
#include "ProjectGE/ResourceManagement/ResourceLoaders/ResourceLoader.h"

namespace ProjectGE {
	class HeapResourceLoader : public ResourceLoader {
	public:
		virtual char* LoadResource(std::string file, UINT* outSize) override;
		// Purpose only for testing
		virtual char* LoadResource(char* data, UINT size) override;
		virtual void CleanResource(char* resource) override;
	};
};

