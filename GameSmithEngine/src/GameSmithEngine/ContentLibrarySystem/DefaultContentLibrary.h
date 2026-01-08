#pragma once
#include "ContentLibraryManager.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	class GE_API DefaultContentLibrary : public ContentLibrary
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		~DefaultContentLibrary() override = default;
	};
};


