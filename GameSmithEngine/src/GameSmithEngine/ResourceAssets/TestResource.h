#pragma once
#include "GameSmithEngine/ResourceManagement/Resource.h"

namespace GameSmith {
	class TestResource : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;
	};
};


