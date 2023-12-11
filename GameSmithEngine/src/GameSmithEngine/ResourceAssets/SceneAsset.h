#pragma once

#include "GameSmithEngine/ResourceManagement/Resource.h"
#include "GameSmithEngine/SceneSystem/Scene.h"

namespace GameSmith {
	class SceneAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		Ref<Scene> GenerateInstance();
	};
};
