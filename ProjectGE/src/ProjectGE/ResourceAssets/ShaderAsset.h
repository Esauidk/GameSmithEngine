#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceManagement/Resource.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace ProjectGE {
	class ShaderAsset : public Resource
	{
	public:
		ShaderAsset() = default;
		inline ShaderAsset(Ref<Shader> shader) : m_Shader(shader) {};
		virtual void Init() override;
		virtual void Destroy() override;

		inline Ref<Shader> GetShader() { return m_Shader != nullptr ? m_Shader : nullptr; }
	private:
		Ref<Shader> m_Shader;
	};
};


