#pragma once
#include "gepch.h"
#include "GameObject.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	struct GameObjectMap {
		std::unordered_map<ID, Ref<GameObject>, IDHasher> objects;
		std::unordered_map<std::string, std::unordered_set<ID, IDHasher>> objectNames;
	};

	class GE_API GameObjectManager {
	public:
		// TODO: Remove this public exposure of the constructor
		GameObjectManager(bool override = false);
		inline static GameObjectManager* GetInstance() { return s_Instance; }

		static void Init();
		static void ShutDown();

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
	private:
		unsigned int m_Counter = 0;

		Scope<GameObjectMap> m_ObjectMaps;
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