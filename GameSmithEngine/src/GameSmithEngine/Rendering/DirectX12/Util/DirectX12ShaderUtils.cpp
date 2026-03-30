#include "gepch.h"
#include "DirectX12ShaderUtils.h"

#define VS_MODEL "vs_6_5"
#define PS_MODEL "ps_6_5"

namespace GameSmith {

	std::string TranslateStageToShaderModel(Stages stage) {
		switch (stage) {
		case STAGE_VERTEX:
			return VS_MODEL;
		case STAGE_PIXEL:
			return PS_MODEL;
		}

		return VS_MODEL;
	}
};


