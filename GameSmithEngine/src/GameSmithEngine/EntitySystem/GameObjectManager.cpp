#include "gepch.h"
#include "GameObjectManager.h"

namespace GameSmith {
	GameObjectManager* GameObjectManager::s_Instance = nullptr;

	GameObjectManager::GameObjectManager() {
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void GameObjectManager::Init()
	{
		m_Counter = 0;
		m_Objects.clear();
		CleanGameObjects();
	}

	void GameObjectManager::ShutDown()
	{
		CleanGameObjects();
	}

	void GameObjectManager::CleanGameObjects()
	{
		// Go through each game object and destroy it
		while (!m_ToBeDeleted.empty()) {
			m_ToBeDeleted.pop();
		}
	}

	Connection<GameObject> GameObjectManager::CreateGameObject(glm::vec3 startingPos, glm::vec3 startingRotation)
	{
		
		std::string objectName;
		if (m_Counter < 1) {
			objectName = GameObject::DEFAULT_GAMEOBJECT_NAME;
		}
		else {
			objectName = std::format("{} ({})", GameObject::DEFAULT_GAMEOBJECT_NAME, m_Counter);
		}

		Ref<GameObject> gameObject = Ref<GameObject>(new GameObject(objectName));
		m_Objects.insert({ objectName, gameObject });
		m_Counter++;

		auto transform = gameObject->GetTransform();
		transform.lock()->SetPosition(startingPos);
		transform.lock()->SetRotation(startingRotation);

		return gameObject;
	}

	Connection<GameObject> GameSmith::GameObjectManager::FindGameObject(std::string gameObjectName)
	{
		if (m_Objects.contains(gameObjectName)) {
			return m_Objects.find(gameObjectName)->second;
		}
		else {
			return Connection<GameObject>();
		}
	}

	void GameObjectManager::DestroyGameObject(Connection<GameObject> object)
	{
		if (!object.expired()) {
			auto temp = object.lock();
			if (temp.get() != nullptr && m_Objects.contains(temp->GetName())) {
				auto item = m_Objects.find(temp->GetName());
				m_ToBeDeleted.push(item->second);
				m_Objects.erase(item);
			}
		}
	}
};

