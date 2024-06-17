#pragma once
#include "EngineDependenciesOnly.h"

namespace GameSmithEditor {
	class EditorCoreLayer : public GameSmith::Layer
	{
	public:
		EditorCoreLayer();
		void OnImGuiRender() override;
		void OnUpdate() override;

		static void RegisterWindow(std::string windowName, std::function<GameSmith::Layer* ()> windowCreator);

		inline static EditorCoreLayer* GetInstance() { return s_Instance; }
	private:
		static EditorCoreLayer* s_Instance;

		GameSmith::Application& m_App;
		GameSmith::Ref<GameSmith::RenderTexture> m_EditorScreen;

		static std::unordered_map<std::string, std::function<GameSmith::Layer* ()>> m_WindowRegistry;
	};
};


