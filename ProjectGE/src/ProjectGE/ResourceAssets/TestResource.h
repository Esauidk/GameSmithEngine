#pragma once
#include "ProjectGE/ResourceManagement/Resource.h"

namespace ProjectGE {
	class TestResource : public Resource
	{
		TestResource(char* data, size_t size) : Resource(data, size) {}
		virtual void Init() override;
		virtual void Destroy() override;
	};
};


