#pragma once
#include "gepch.h"

namespace ProjectGE {
	class ResourceLoader {
	public:
		virtual char* LoadResource(std::string file, UINT* outSize) = 0;
		virtual void CleanResource(char* resource) = 0;
	};
};
