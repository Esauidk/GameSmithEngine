#pragma once
#include "GameSmithEngine/Core/Core.h"

#define GE_IMPL_CONNECT_CONTENTLIBRARY(ContentLibraryClass) \
extern "C" GE_EXPLCIT_EXPORT GameSmith::ContentLibrary* ConnectToContentLibrary() { \
	return new ContentLibraryClass(); \
}\

namespace GameSmith {
	class GE_API ContentLibrary {
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual ~ContentLibrary() = 0;
	};
};
