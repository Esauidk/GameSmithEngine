#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"

namespace GameSmith {
	class GameChunk
	{
	public:
		GameChunk(std::vector<Connection<GameObject>> objects) : m_GameObjects(objects) {};
		~GameChunk();
	private:
		std::vector<Connection<GameObject>> m_GameObjects;
	};
};


