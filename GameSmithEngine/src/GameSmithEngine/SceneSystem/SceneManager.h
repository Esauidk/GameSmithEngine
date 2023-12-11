#pragma once

#include "GameSmithEngine/SceneSystem/Scene.h"
#include "GameSmithEngine/ResourceAssets/SceneAsset.h"

namespace GameSmith {
	class SceneManager
	{
	public:
		inline static SceneManager* GetInstance() { return s_Instance; }
		void Init() {};
		void ShutDown() {};

		void LoadScene(std::string sceneAsset);
		// Visible for testing
		void LoadScene(std::string sceneAsset, Ref<SceneAsset> asset);
	private:
		static SceneManager* s_Instance;

		std::unordered_map<std::string, Ref<Scene>> m_LoadedScenes;
		std::unordered_map<std::string, Ref<SceneAsset>> m_LoadedSceneAssets;
	};
};


