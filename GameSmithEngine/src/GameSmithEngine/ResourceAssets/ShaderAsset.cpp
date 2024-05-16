#include "gepch.h"
#include "ShaderAsset.h"

#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {

	Ref<char> ShaderAsset::Serialize()
	{
		// TODO: Implement
		return Ref<char>();
	}
	void ShaderAsset::Serialize(Ref<char> byteStream, unsigned int availableBytes)
	{
		// TODO: Implement
	}

	unsigned int ShaderAsset::RequireSpace() const
	{
		// TODO: Implement
		return 0;
	}

	void ShaderAsset::Deserialize(char* inData, unsigned int size)
	{
		auto renderAPI = RenderingManager::GetInstance()->GetRenderAPI();
		m_Shader = renderAPI->LoadShader(inData, size);
	}
};

