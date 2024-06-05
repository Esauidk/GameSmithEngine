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

		Connection<GameObject> CreateGameObject(glm::vec3 startingPos = glm::vec3(0), glm::vec3 startingRotation = glm::vec3(0));
		Connection<GameObject> FindGameObject(std::string gameObjectName);
		void GetGameObjects(std::vector<Connection<GameObject>>* outObjects);
		void GetGameObjectNames(std::vector<std::string>* outNames);
		void DestroyGameObject(Connection<GameObject> object);
	private:
		friend class GameObjectProxy;
		void UpdateGameObjectName(std::string newName, Connection<GameObject> targetObject);
	private:
		static GameObjectManager* s_Instance;

		unsigned int m_Counter = 0;


		std::unordered_map<ID, Ref<GameObject>, ID> m_Objects;
		std::unordered_map<std::string, std::unordered_set<ID, ID>> m_ObjectNames;
		std::queue<Ref<GameObject>> m_ToBeDeleted;
	};

	class GameObjectProxy {
	private:
		inline static void UpdateManagerStoredNames(std::string newName, Connection<GameObject> targetObject) {
			auto manager = GameObjectManager::GetInstance();
			manager->UpdateGameObjectName(newName, targetObject);
		};

		friend class GameObject;
	};
};