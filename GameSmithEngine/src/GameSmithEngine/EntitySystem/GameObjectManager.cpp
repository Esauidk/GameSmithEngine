#include "gepch.h"
#include "GameObjectManager.h"

namespace GameSmith {
	GameObjectManager* GameObjectManager::s_Instance = nullptr;

	GameObjectManager::GameObjectManager(bool override) : m_ObjectMaps(new GameObjectMap()) {
	}

	void GameObjectManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new GameObjectManager();
			s_Instance->m_Counter = 0;
			s_Instance->m_ObjectMaps->objects.clear();
			s_Instance->CleanGameObjects();
		}
		
	}

	void GameObjectManager::ShutDown()
	{
		if (s_Instance != nullptr) {
			s_Instance->CleanGameObjects();
			delete s_Instance;
			s_Instance = nullptr;
		}
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
		m_ObjectMaps->objects.insert({ gameObject->GetID(), gameObject });
		m_Counter++;

		if (m_ObjectMaps->objectNames.contains(objectName)) {
			auto entry = m_ObjectMaps->objectNames.find(objectName);
			entry->second.insert(gameObject->GetID());
		}
		else {
			m_ObjectMaps->objectNames.insert({ objectName, {gameObject->GetID()} });
		}

		auto transform = gameObject->GetTransform();
		transform.lock()->SetPosition(startingPos);
		transform.lock()->SetRotation(startingRotation);

		return gameObject;
	}

	Connection<GameObject> GameSmith::GameObjectManager::FindGameObject(std::string gameObjectName)
	{
		// TODO: Fix this when a solution for String -> Is completed
		if (m_ObjectMaps->objectNames.contains(gameObjectName)) {
			auto& id = *(m_ObjectMaps->objectNames.find(gameObjectName)->second.begin());
			return m_ObjectMaps->objects.find(id)->second;
		}
		else {
			return Connection<GameObject>();
		}
	}

	void GameObjectManager::GetGameObjects(std::vector<Connection<GameObject>>* outObjects)
	{
		for (auto object : m_ObjectMaps->objects) {
			outObjects->push_back(object.second);
		}
	}

	void GameObjectManager::GetGameObjectNames(std::vector<std::string>* outNames)
	{
		for (auto object : m_ObjectMaps->objectNames) {
			outNames->push_back(object.first);
		}
	}

	void GameObjectManager::DestroyGameObject(Connection<GameObject> object)
	{
		if (!object.expired()) {
			auto temp = object.lock();
			if (temp.get() != nullptr && m_ObjectMaps->objects.contains(temp->GetID())) {
				auto objItem = m_ObjectMaps->objects.find(temp->GetID());
				auto nameItem = m_ObjectMaps->objectNames.find(temp->GetName());

				if (nameItem->second.contains(temp->GetID())) {
					nameItem->second.erase(temp->GetID());

					if (nameItem->second.size() == 0) {
						m_ObjectMaps->objectNames.erase(temp->GetName());
					}
				}


				m_ToBeDeleted.push(objItem->second);
				m_ObjectMaps->objects.erase(objItem);
			}
		}
	}

	void GameObjectManager::UpdateGameObjectName(std::string newName, Connection<GameObject> targetObject)
	{
		if (!targetObject.expired()) {
			auto temp = targetObject.lock();
			if (temp.get() != nullptr && m_ObjectMaps->objectNames.contains(temp->GetName())) {
				auto item = m_ObjectMaps->objectNames.find(temp->GetName());
				
				auto ID = temp->GetID(); 
				if (item->second.contains(ID)) {
					item->second.erase(ID);

					if (item->second.size() == 0) {
						m_ObjectMaps->objectNames.erase(temp->GetName());
					}
				}

				if (m_ObjectMaps->objectNames.contains(newName)) {
					m_ObjectMaps->objectNames.find(newName)->second.insert(ID);
				}
				else {
					m_ObjectMaps->objectNames.insert({ newName, {ID} });
				}
			}
		}
	}
};

