#pragma once
#include "ProjectGE/Core/Core.h"

#include "ProjectGE/Rendering/BindableResource.h"

namespace ProjectGE {
	class Texture : public BindableResource
	{
	public:
		virtual UINT GetWidth() const = 0;
		virtual UINT GetHeight() const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
};


