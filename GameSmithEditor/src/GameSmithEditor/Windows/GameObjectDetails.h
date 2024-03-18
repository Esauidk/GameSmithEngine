#pragma once
#include "GameSmithEditor/EngineDependenciesOnly.h"

namespace GameSmithEditor {
	class GameObjectDetails : public GameSmith::Layer
	{
	public:
		void OnImGuiRender() override;
		void OnUpdate() override;
	private:
		GameSmith::Connection<GameSmith::GameObject> m_Object;
		std::vector<GameSmith::Connection<GameSmith::Component>> m_Components;
	};
};


