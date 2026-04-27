#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

#define DEFAULT_SENSITIVITY 1
namespace GameSmithEditor {
	class EditorView : public EditorWindow {
	public:
		EditorView();
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		static GameSmith::Ref<GameSmith::RenderTexture> s_EditorScreenTexture;
		static GameSmith::PerspectiveCamera s_PerpCamera;
		static GameSmith::OrthoCamera s_OrthoCamera;
	private:
		const GameSmith::ImGuiTextureSpace* m_EditorViewHandle;
		bool m_Open = true;

		bool m_Dragging;
		glm::vec2 m_DragDir;

		float m_DragSensitivity = DEFAULT_SENSITIVITY;
	};
};
