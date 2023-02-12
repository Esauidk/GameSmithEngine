#pragma once
#include "BindableResource.h"
#include <string>
#include <wrl.h>
#include <d3dcompiler.h>


namespace Render {
	class Shader : public BindableResource
	{
	public:
		Shader(const std::wstring shaderPath);
	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
	};
};


