#pragma once
#include "ProjectGE/ResourceManagement/Resource.h"

namespace ProjectGE {
	class TestResource : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;
	};
};


