#pragma once
#include "GameSmithEngine.h"
#include "GameSmithEditor/Windows/EditorWindowEvents.h"

#define EDITOR_WINDOW_TITLE "Game Smith Editor"

namespace GameSmithEditor {
	class EditorWindow : public GameSmith::Layer {
	public:
		EditorWindow(std::string name) : GameSmith::Layer(name) {}
		GameSmith::EventDispatcher<EditorWindowClose>& GetCloseEventDispatch() { return m_CloseDispatcher; }
	protected:
		void CloseWindow();
	private:
		GameSmith::EventDispatcher<EditorWindowClose> m_CloseDispatcher;
	};

	class EditorCoreLayer : public GameSmith::Layer
	{
	public:
		EditorCoreLayer();

		void OnAttach(const GameSmith::ApplicationSpecs& specs) override;
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;

		template<typename W>
		static void CreateEditorWindow() {
			EditorWindow* window = new W();

			window->GetCloseEventDispatch().AddListener(
				[window](EditorWindowClose& evn) {
					auto& app = GameSmith::Application::Get();
					app.PopLayer(window);

					return true;
				}, false
			);

			auto& app = GameSmith::Application::Get();
			app.PushLayer(window);
		}

		inline static EditorCoreLayer* GetInstance() { return s_Instance; }
	private:
		static EditorCoreLayer* s_Instance;
		static std::vector<GameSmith::Ref<GameSmith::Layer>> m_EditorWindows;

		GameSmith::Application& m_App;
		GameSmith::Ref<GameSmith::RenderTexture> m_EditorScreen;
	};
};


