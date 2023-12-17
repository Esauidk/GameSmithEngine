#pragma once

#include "GameSmithEngine/ResourceManagement/Resource.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"

namespace GameSmith {
	class GameChunkAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		Ref<GameChunk> GenerateInstance();
	};
};
