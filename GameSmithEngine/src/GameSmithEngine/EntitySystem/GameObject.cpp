#include "gepch.h"
#include "GameObject.h"
#include "Transform.h"
#include "Components/ComponentFactory.h"
#include "GameObjectManager.h"

namespace GameSmith {
	const std::string GameObject::DEFAULT_GAMEOBJECT_NAME = "GameObject";

	GameObject::GameObject(std::string name) : m_Name(name), m_Transform(new Transform()) {}

	void GameObject::SetName(std::string newName)
	{
		auto con = GameObjectManager::GetInstance()->FindGameObject(m_Name);
		m_Name = newName;
		GameObjectProxy::UpdateManagerStoredNames(newName, con);
	}

	Connection<Component> GameObject::AddComponent(std::string componentName)
	{
		auto comp = ComponentFactory::GenerateComponent(componentName, this, m_Transform.get());

		auto updater = GameplayUpdater::GetInstance();

		if (updater != nullptr) {
			updater->Register(comp);
		}
		else {
			GE_CORE_ERROR("Unable to get reference to gameplay updater");
		}

		m_Components.emplace_back(comp);

		return comp;
	}
};