#pragma once
#include "GameSmithEngine.h"

#define EDITOR_WINDOW_TITLE "Game Smith Editor"

namespace GameSmithEditor {
	class EditorCoreLayer : public GameSmith::Layer
	{
	public:
		EditorCoreLayer();

		void OnAttach() override;
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;

		template<typename W>
		static void CreateEditorWindow() {
			W* window = new W();

			auto& app = GameSmith::Application::Get();
			app.PushLayer(window);
		}

		inline static EditorCoreLayer* GetInstance() { return s_Instance; }
	private:
		static EditorCoreLayer* s_Instance;

		GameSmith::Application& m_App;
		GameSmith::Ref<GameSmith::RenderTexture> m_EditorScreen;
	};
};


