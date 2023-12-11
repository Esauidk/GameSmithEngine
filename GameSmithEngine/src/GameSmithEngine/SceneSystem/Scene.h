#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"

namespace GameSmith {
	class Scene
	{
	public:
		//Scene(std::vector<>)
		// Intended to be used in testing
		inline void AddGameObject(Ref<GameObject> newObject) { m_GameObjects.push_back(newObject); };
	private:
		std::vector<Ref<GameObject>> m_GameObjects;
	};
};


