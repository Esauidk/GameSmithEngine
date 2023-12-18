#pragma once
#include "GameObject.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	class GameObjectManager {
	public:
		GameObjectManager(bool override = false);
		inline static GameObjectManager* GetInstance() { return s_Instance; }

		void Init();
		void ShutDown();
		void CleanGameObjects();

		Connection<GameObject> CreateGameObject(glm::vec3 startingPos, glm::vec3 startingRotation);
		Connection<GameObject> FindGameObject(std::string gameObjectName);
		void DestroyGameObject(Connection<GameObject> object);

	private:
		static GameObjectManager* s_Instance;

		unsigned int m_Counter = 0;

		std::unordered_map<std::string, Ref<GameObject>> m_Objects;
		std::queue<Ref<GameObject>> m_ToBeDeleted;
	};
};