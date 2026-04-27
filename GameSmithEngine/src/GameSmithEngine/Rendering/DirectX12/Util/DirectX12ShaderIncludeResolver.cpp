#include "gepch.h"
#include "DirectX12ShaderIncludeResolver.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Utilities/MiscellaneousUtilities.h"
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace GameSmith {
	HRESULT DirectX12ShaderIncludeResolver::LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource)
	{
		GE_CORE_ASSERT(m_IncludeCache != nullptr, "No Shader Cache has been set in the include resolver");

		const std::wstring fileNameW(pFilename);
		const std::string fileName = ToNarrowString(fileNameW);
		const std::string cleanedFileName = fileName.find_last_of("\\/") != std::string::npos ? fileName.substr(fileName.find_last_of("\\/") + 1) : fileName;

		unsigned int sourceSize = 0;
		const char* source = m_IncludeCache->RetrieveShaderContent(cleanedFileName, &sourceSize);

		IDxcBlobEncoding* blob;
		m_Utils->CreateBlob(
			source,
			sourceSize,
			DXC_CP_UTF8,
			&blob
		);

		*ppIncludeSource = blob;

		return S_OK;
	}
};

