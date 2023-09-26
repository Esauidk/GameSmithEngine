#pragma once

namespace ProjectGE {
	struct TextureMetadata {
		UINT width;
		UINT height;
		UINT depth;
		UINT channels;
		UINT mips;
	};

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
	};
};


