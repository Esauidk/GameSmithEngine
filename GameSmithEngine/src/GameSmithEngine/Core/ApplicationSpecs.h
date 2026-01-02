#pragma once
#include "GameSmithEngine/Core/Core.h"
#include <string>

namespace GameSmith {
	struct GE_API CommandLineArgs {
		int Count = 0;
		char** Args = nullptr;
	};

	struct GE_API ApplicationSpecs {
		std::string Name = "Game Smith Engine";
		CommandLineArgs CommandLineArgs;
	};
}
