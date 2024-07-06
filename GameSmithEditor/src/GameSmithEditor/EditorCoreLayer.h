#pragma once
#include "EngineDependenciesOnly.h"

#define GETR_REGISTEREDITORWINDOW(ClassType) \
	 static struct ClassType##RegisterAction { \
		ClassType##RegisterAction() { \
			GameSmithEditor::EditorCoreLayer::RegisterWindow( \
				#ClassType, []() {return new ClassType(); } \
			); \
		} \
	}ClassType##Instance;

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


