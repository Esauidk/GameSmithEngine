#pragma once
#include "GameSmithEngine/ResourceManagement/ResourceLoaders/ResourceLoader.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	class GE_API HeapResourceLoader : public ResourceLoader {
	public:
		virtual char* LoadResource(std::string file, UINT* outSize) override;
		// Purpose only for testing
		virtual char* LoadResource(char* data, UINT size) override;
		virtual void CleanResource(char* resource) override;
	};
};

