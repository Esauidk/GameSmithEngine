#include "gepch.h"
#include "GameObjectManager.h"

namespace GameSmith {
	GameObjectManager* GameObjectManager::s_Instance = nullptr;

	GameObjectManager::GameObjectManager(bool override) {
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
		else if (override) {
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
		m_Objects.insert({ gameObject->GetId(), gameObject });
		m_Counter++;

		if (m_ObjectNames.contains(objectName)) {
			auto entry = m_ObjectNames.find(objectName);
			entry->second.insert(gameObject->GetId());
		}
		else {
			m_ObjectNames.insert({ objectName, {gameObject->GetId()} });
		}

		auto transform = gameObject->GetTransform();
		transform.lock()->SetPosition(startingPos);
		transform.lock()->SetRotation(startingRotation);

		return gameObject;
	}

	Connection<GameObject> GameSmith::GameObjectManager::FindGameObject(std::string gameObjectName)
	{
		// TODO: Fix this when a solution for String -> Is completed
		if (m_ObjectNames.contains(gameObjectName)) {
			auto& id = *(m_ObjectNames.find(gameObjectName)->second.begin());
			return m_Objects.find(id)->second;
		}
		else {
			return Connection<GameObject>();
		}
	}

	void GameObjectManager::GetGameObjects(std::vector<Connection<GameObject>>* outObjects)
	{
		for (auto object : m_Objects) {
			outObjects->push_back(object.second);
		}
	}

	void GameObjectManager::GetGameObjectNames(std::vector<std::string>* outNames)
	{
		for (auto object : m_ObjectNames) {
			outNames->push_back(object.first);
		}
	}

	void GameObjectManager::DestroyGameObject(Connection<GameObject> object)
	{
		if (!object.expired()) {
			auto temp = object.lock();
			if (temp.get() != nullptr && m_Objects.contains(temp->GetId())) {
				auto objItem = m_Objects.find(temp->GetId());
				auto nameItem = m_ObjectNames.find(temp->GetName());

				if (nameItem->second.contains(temp->GetId())) {
					nameItem->second.erase(temp->GetId());

					if (nameItem->second.size() == 0) {
						m_ObjectNames.erase(temp->GetName());
					}
				}


				m_ToBeDeleted.push(objItem->second);
				m_Objects.erase(objItem);
			}
		}
	}

	void GameObjectManager::UpdateGameObjectName(std::string newName, Connection<GameObject> targetObject)
	{
		if (!targetObject.expired()) {
			auto temp = targetObject.lock();
			if (temp.get() != nullptr && m_ObjectNames.contains(temp->GetName())) {
				auto item = m_ObjectNames.find(temp->GetName());
				
				auto ID = temp->GetId(); 
				if (item->second.contains(ID)) {
					item->second.erase(ID);

					if (item->second.size() == 0) {
						m_ObjectNames.erase(temp->GetName());
					}
				}

				if (m_ObjectNames.contains(newName)) {
					m_ObjectNames.find(newName)->second.insert(ID);
				}
				else {
					m_ObjectNames.insert({ newName, {ID} });
				}
			}
		}
	}
};

