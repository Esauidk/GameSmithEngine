#include "gepch.h"
#include "Shader.h"

std::string GameSmith::ConvertShaderStageToString(Stages stage)
{
    switch (stage) {
        case STAGE_VERTEX:
			return "Vertex";
		case STAGE_HULL:
			return "Hull";
		case STAGE_DOMAIN:
			return "Domain";
		case STAGE_PIXEL:
			return "Pixel";
    }
    return std::string();
}
