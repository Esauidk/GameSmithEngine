#include "gepch.h"
#include "GameObject.h"
#include "Components/Transform.h"

namespace GameSmith {
	const std::string GameObject::DEFAULT_GAMEOBJECT_NAME = "GameObject";

	GameObject::GameObject(std::string name) : m_Name(name)
	{
		m_Transform = Ref<Transform>(new Transform());
	}

	void GameObject::OnUpdate()
	{
		m_Transform->OnUpdate();
		for (auto comp : m_Components) {
			comp->OnUpdate();
		}
	}
};