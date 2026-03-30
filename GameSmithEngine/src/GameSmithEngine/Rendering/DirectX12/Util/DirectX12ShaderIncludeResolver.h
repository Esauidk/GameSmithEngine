#pragma once

#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderIncludeCache.h"
#include <dxcapi.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace GameSmith {
	class DirectX12ShaderIncludeResolver : public IDxcIncludeHandler
	{
	public:
		DirectX12ShaderIncludeResolver(ComPtr<IDxcUtils> utilInstance) : m_IncludeCache(nullptr), m_Utils(utilInstance) {}

		inline void SetIncludeCache(const ShaderIncludeCache* cache) { m_IncludeCache = cache; }

		HRESULT LoadSource(LPCWSTR  pFilename, IDxcBlob** ppIncludeSource) override;

		HRESULT QueryInterface(REFIID riid, void** ppvObject) override
		{
			if (riid == __uuidof(IDxcIncludeHandler)) {
				*ppvObject = static_cast<IDxcIncludeHandler*>(this);
				AddRef();
				return S_OK;
			}
			else if (riid == __uuidof(IUnknown)) {
				*ppvObject = static_cast<IUnknown*>(this);
				AddRef();
				return S_OK;
			}
			else {
				*ppvObject = nullptr;
				return E_NOINTERFACE;
			}
		}

		ULONG AddRef() override
		{
			m_RefCount++;
			return m_RefCount;
		}

		ULONG Release() override
		{
			m_RefCount--;
			return m_RefCount;
		}
	private:
		const ShaderIncludeCache* m_IncludeCache;
		ComPtr<IDxcUtils> m_Utils;

		unsigned int m_RefCount = 0;
	};
}