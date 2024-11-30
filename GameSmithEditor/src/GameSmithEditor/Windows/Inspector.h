#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

#define GAMEOBJECT_NAME_SIZE 500
namespace GameSmithEditor {
	class Inspector : public EditorWindow
	{
	public:
		Inspector() : EditorWindow("Inspector") {}
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		GameSmith::Connection<GameSmith::GameObject> m_Object;
		GameSmith::Connection<GameSmith::Serializeable> m_Asset;


		std::vector<GameSmith::Connection<GameSmith::Component>> m_Components;
		std::unordered_map<std::string, std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>>> m_ExposedVariables;
		std::unordered_map<std::string, std::unordered_map<std::string, GameSmith::Ref<GameSmith::ConnectionContainer>>> m_ExposedRefs;
		std::unordered_map<std::string, std::unordered_map<std::string, GameSmith::Ref<GameSmith::AssetRefContainer>>> m_ExposedAssets;
		std::string m_CurCompSelection;
		char m_InputName[GAMEOBJECT_NAME_SIZE] = "";

	public:
		static void SetInspectedGameObject(GameSmith::Connection <GameSmith::GameObject> object);
	private:
		static GameSmith::Connection<GameSmith::GameObject> s_SelectedObject;
	};
};


