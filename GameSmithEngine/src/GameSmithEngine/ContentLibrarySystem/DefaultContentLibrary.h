#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "ContentLibraryManager.h"

namespace GameSmith {
	class GE_API DefaultContentLibrary : public ContentLibrary
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
	};
};


