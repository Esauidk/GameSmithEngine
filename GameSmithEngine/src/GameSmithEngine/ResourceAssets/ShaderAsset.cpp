#include "gepch.h"
#include "ShaderAsset.h"

#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {
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

