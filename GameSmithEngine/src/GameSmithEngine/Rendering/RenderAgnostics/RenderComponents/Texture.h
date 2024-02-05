#pragma once
#include "GameSmithEngine/Events/ApplicationEvents.h"

namespace GameSmith {
	struct TextureMetadata {
		UINT width;
		UINT height;
		UINT depth;
		UINT channels;
		UINT mips;
		float clearColor[4] = { 0, 0, 0, 0 };
	};

	enum class TextureType {
		Tex2D
	};

	enum class TextureMisc {
		RT,
		DT,
		Normal
	};

	class Texture
	{
	public:
		virtual UINT GetWidth() const = 0;
		virtual UINT GetHeight() const = 0;
	};

	class Texture2D : public Texture {
	};

	class RenderTexture : public Texture {
	public:
		virtual void ClearTexture() = 0;
		virtual bool WindowResized(WindowResizeEvent& e) = 0;
		virtual const float* GetClearColor() const = 0;
	};
};


