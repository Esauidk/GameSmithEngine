#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

#define GAMEOBJECT_NAME_SIZE 500
namespace GameSmithEditor {
	class GameObjectDetails : public EditorWindow
	{
	public:
		GameObjectDetails() : EditorWindow("GameObjectDetails") {}
		void OnImGuiRender() override;
		void OnUpdate() override;
	private:
		GameSmith::Connection<GameSmith::GameObject> m_Object;
		std::vector<GameSmith::Connection<GameSmith::Component>> m_Components;
		std::unordered_map<std::string, std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>>> m_ExposedVariables;
		std::string m_CurCompSelection;
		char m_InputName[GAMEOBJECT_NAME_SIZE] = "";
	};
};


