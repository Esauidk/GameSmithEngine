#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "DirectX12ShaderCompiler.h"
#include "GameSmithEngine/Utilities/MiscellaneousUtilities.h"

#pragma comment(lib, "dxcompiler.lib")

// TODO: Revisit optimization settings and make them configurable based on debug/release and other settings
// Starting optimization flags
#ifdef GE_DEBUG
	#define EMBED_DEBUG_INFO L"-Qembed_debug"
	#define SHADER_COMPILATION_FLAGS { DXC_ARG_ENABLE_STRICTNESS, DXC_ARG_OPTIMIZATION_LEVEL2, DXC_ARG_DEBUG, EMBED_DEBUG_INFO }
#else
	#define SHADER_COMPILATION_FLAGS { DXC_ARG_ENABLE_STRICTNESS, DXC_ARG_OPTIMIZATION_LEVEL2 }
#endif



namespace GameSmith {
	DirectX12ShaderCompiler::DirectX12ShaderCompiler()
	{
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_DxCompiler));
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_DxUtils));

		m_IncludeResolver = new DirectX12ShaderIncludeResolver(m_DxUtils);
	}

	Ref<const char> DirectX12ShaderCompiler::CompileShader(
		const char* source,
		const unsigned int sourceLength,
		const std::string entryPoint,
		const std::string profile,
		const ShaderIncludeCache* cache,
		unsigned int* outSize)
	{
		m_IncludeResolver->SetIncludeCache(cache);

		const DxcBuffer sourceBuffer = {
			source,
			sourceLength,
			DXC_CP_UTF8
		};

		LPCWSTR arguments[] = SHADER_COMPILATION_FLAGS;

		const std::wstring entryPointW = ToWideString(entryPoint);
		const std::wstring profileW = ToWideString(profile);

		ComPtr<IDxcCompilerArgs> compilerArgs;
		m_DxUtils->BuildArguments(
			L"Shader",
			entryPointW.c_str(),
			profileW.c_str(),
			arguments,
			ARRAYSIZE(arguments),
			nullptr,
			0,
			&compilerArgs
		);

		ComPtr<IDxcResult> result;
		m_DxCompiler->Compile(
			&sourceBuffer,
			compilerArgs->GetArguments(),
			compilerArgs->GetCount(),
			m_IncludeResolver.Get(),
			IID_PPV_ARGS(&result)
		);

		HRESULT hr;
		result->GetStatus(&hr);
		if (!FAILED(hr)) {
			GE_CORE_ASSERT(result->HasOutput(DXC_OUT_OBJECT), "Compiled shader was not returned");

			ComPtr<IDxcBlob> compiledShader;
			hr = result->GetOutput(
				DXC_OUT_OBJECT,
				IID_PPV_ARGS(&compiledShader),
				nullptr
			);

			GE_CORE_ASSERT(!FAILED(hr), "Failed to retrieve compiled shader blob");

			char* compiledCode = new char[compiledShader->GetBufferSize()];
			memcpy(compiledCode, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize());

			*outSize = static_cast<unsigned int>(compiledShader->GetBufferSize());
			return Ref<const char>(compiledCode);
		}
		else {
			std::string errorMsg = "";
			if (result->HasOutput(DXC_OUT_ERRORS)) {
				ComPtr<IDxcBlobUtf8> errors;
				hr = result->GetOutput(
					DXC_OUT_ERRORS,
					IID_PPV_ARGS(&errors),
					nullptr
				);

				GE_CORE_ASSERT(!FAILED(hr), "Failed to retrieve shader compilation error message");
				errorMsg = std::string(errors->GetStringPointer(), errors->GetStringLength());
			}

			GE_CORE_ERROR("Shader compilation failed: {0}", errorMsg);
		}
		
		
		return Ref<const char>();
	}
};