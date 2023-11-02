#include "gepch.h"
#include "GameObject.h"
#include "Components/Transform.h"

namespace ProjectGE {
	GameObject::GameObject()
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