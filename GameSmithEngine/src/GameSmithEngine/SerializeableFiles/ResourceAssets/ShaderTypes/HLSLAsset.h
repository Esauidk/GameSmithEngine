#pragma once
#include "../BlobAsset.h"

namespace GameSmith {
	class GE_API HLSLAsset : public BlobAsset {
	public:
		HLSLAsset(std::string fileName) : BlobAsset(fileName) {}
		SERIAL_FILE(HLSL, hlsl)
	};
};