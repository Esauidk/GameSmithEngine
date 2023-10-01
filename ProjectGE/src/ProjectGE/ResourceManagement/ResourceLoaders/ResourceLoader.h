#pragma once
#include "gepch.h"

namespace ProjectGE {
	class ResourceLoader {
	public:
		virtual char* LoadResource(std::string file, size_t* outSize) = 0;
	};
};
