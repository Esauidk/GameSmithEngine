#pragma once
#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	enum class TextureType {
		Tex2D
	};

	class Texture
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


