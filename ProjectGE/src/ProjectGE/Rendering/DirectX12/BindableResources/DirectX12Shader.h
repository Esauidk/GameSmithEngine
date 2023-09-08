#pragma once
#include "gepch.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/ShaderInputs/ShaderParameterGroup.h"

using Microsoft::WRL::ComPtr;

namespace ProjectGE {
	class DirectX12Shader : public Shader
	{
	public:
		DirectX12Shader(const std::string path);
		inline ID3DBlob* ByteCode() const { return m_Blob.Get(); }
		void Bind() override;
	private:
		void InspectShader();
	private:
		ComPtr<ID3DBlob> m_Blob;
		ComPtr<ID3D12ShaderReflection> m_Reflect;
		std::unordered_map<std::string, Ref<ShaderParameterGroup>> m_GroupMapping;

	};
};


