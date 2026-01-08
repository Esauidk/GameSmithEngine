#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "GUIDGenerator.h"

namespace GameSmith {
	ID GameSmith::GUIDGenerator::GenerateID()
	{
#ifdef GE_PLATFORM_WINDOWS
		GUID id;
		int hres = FAILED(CoCreateGuid(&id));

		GE_CORE_ASSERT(!hres, "Could not generate GUID");

		return ID(id.Data1, id.Data2, id.Data3, *((unsigned long long*)id.Data4));
#endif

		return ID(0,0,0,0);
	}
};


