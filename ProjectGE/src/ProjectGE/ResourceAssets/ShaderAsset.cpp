#include "gepch.h"
#include "ShaderAsset.h"

#include "ProjectGE/Rendering/RenderingManager.h"

namespace ProjectGE {
	void ShaderAsset::Init()
	{
		auto renderAPI = RenderingManager::GetInstance()->GetRenderAPI();
		m_Shader = renderAPI->LoadShader(GetResourceData(), GetResourceSize());
	}

	void ShaderAsset::Destroy()
	{
		m_Shader = nullptr;
	}
};

