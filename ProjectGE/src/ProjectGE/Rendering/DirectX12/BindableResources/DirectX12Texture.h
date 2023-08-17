#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Texture.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Buffer.h"

namespace ProjectGE {
	class DirectX12Texture2D : public Texture2D
	{
	public :
		DirectX12Texture2D(const std::string& path);

		UINT GetWidth() const override { return m_Widght; };
		UINT GetHeight() const override { return m_Height; };

		virtual void Bind() override;

	private:
		Scope<DirectX12Buffer<BYTE>> m_Buffer;
		std::string m_Path;
		UINT m_Widght;
		UINT m_Height;
	};
};


