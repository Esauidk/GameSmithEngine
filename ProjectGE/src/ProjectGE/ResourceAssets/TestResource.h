#pragma once
#include "ProjectGE/ResourceManagement/Resource.h"

namespace ProjectGE {
	class TestResource : public Resource
	{
	public:
		TestResource(char* data, UINT size) : Resource(data, size) {}
		virtual void Init() override;
		virtual void Destroy() override;
	};
};


