#include "EditorView.h"
#include "imgui.h"

namespace GameSmithEditor {
	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_EditorView, EditorView);

	GameSmith::Ref<GameSmith::RenderTexture> EditorView::s_EditorScreenTexture = nullptr;
	GameSmith::PerspectiveCamera EditorView::s_PerpCamera = GameSmith::PerspectiveCamera(1, 1, 1, 1, 1);
	GameSmith::OrthoCamera EditorView::s_OrthoCamera = GameSmith::OrthoCamera();

	EditorView::EditorView() : EditorWindow("Editor View") {
		auto& app = GameSmith::Application::Get();
		auto renderManager = GameSmith::RenderingManager::GetInstance();
		if (s_EditorScreenTexture == nullptr) {
			float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
			s_EditorScreenTexture = renderManager->GetRenderAPI()->CreateRenderTexture(app.GetWindow()->GetWidth(), app.GetWindow()->GetHeight(), color);
			GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, s_EditorScreenTexture.get()), false);
			s_PerpCamera = GameSmith::PerspectiveCamera();
		}
		
		m_EditorViewHandle = app.GetImGuiInstance()->GenerateTextureSpace(s_EditorScreenTexture);
	}

	void EditorView::OnImGuiRender()
	{
		if (ImGui::Begin("Editor View", &m_Open)) {
			ImTextureRef texRef = ImTextureRef((ImTextureID)m_EditorViewHandle->gpuSpot);
			ImGui::Image(texRef, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));

			if (ImGui::IsWindowFocused()) {
				m_Dragging = ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Middle);
				if (m_Dragging) {
					const ImVec2 drag = ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Middle);
					m_DragDir = glm::vec2(drag.x, drag.y);
					GE_APP_INFO("DRAGGING: ({0}, {1})", drag.x, drag.y);
				}
			}
		}
		ImGui::End();
	}

	void EditorView::OnUpdate(float dt)
	{
		if (!m_Open) {
			CloseWindow();
		}

		auto entityPreparer = GameSmith::EntityRenderPreparer::GetInstance();
		GameSmith::RenderableCamera renderCam;
		renderCam.cam = &s_PerpCamera;
		renderCam.targetTex = s_EditorScreenTexture;
		entityPreparer->AddAdditionalCamera(renderCam);

		if (m_Dragging) {
			auto& perCamTrans = s_PerpCamera.GetTransform();
			glm::vec3 curPosition = perCamTrans.GetPosition();
			glm::vec2 camChange = m_DragDir * m_DragSensitivity * dt;

			glm::vec3 newPosition = glm::vec3(curPosition.x + camChange.x, curPosition.y + camChange.y, curPosition.z);
			perCamTrans.SetPosition(newPosition);
		}

		auto renderManager = GameSmith::RenderingManager::GetInstance();
		renderManager->SetForClear(s_EditorScreenTexture);
	}
};