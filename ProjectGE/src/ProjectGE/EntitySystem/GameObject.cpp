#include "gepch.h"
#include "GameObject.h"
#include "Components/Transform.h"

namespace ProjectGE {
	GameObject::GameObject()
	{
		AddComponent<Transform>();
	}
};