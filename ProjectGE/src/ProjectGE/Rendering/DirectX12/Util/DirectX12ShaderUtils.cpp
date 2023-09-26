#include "gepch.h"
#include "DirectX12ShaderUtils.h"

#define VS_MODEL "vs_5_0"
#define PS_MODEL "ps_5_0"

namespace ProjectGE {

	static std::string TranslateStageToShaderModel(Stages stage) {
		switch (stage) {
		case STAGE_VERTEX:
			return VS_MODEL;
		case STAGE_PIXEL:
			return PS_MODEL;
		}

		return VS_MODEL;
	}

	ComPtr<ID3DBlob> CompileShaderForDX12(std::string rawCode, std::string entryFunction, Stages stage, std::string destFile)
	{
		ComPtr<ID3DBlob> resultShader;
		std::string shaderModel = TranslateStageToShaderModel(stage);
		D3DCompile2(rawCode.c_str(), rawCode.size(), NULL, NULL, NULL, entryFunction.c_str(), shaderModel.c_str(), NULL, NULL, NULL, NULL, NULL, &resultShader, NULL);

		return resultShader;
	}
};


