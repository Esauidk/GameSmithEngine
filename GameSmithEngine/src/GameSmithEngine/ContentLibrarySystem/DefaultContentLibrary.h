#pragma once
#include "ContentLibraryManager.h"

namespace GameSmith {
	class DefaultContentLibrary : public ContentLibrary
	{
		virtual void Init() override;
		virtual void Shutdown() override;
	};
};


