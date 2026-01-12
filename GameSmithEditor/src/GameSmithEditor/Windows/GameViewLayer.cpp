#include "GameViewLayer.h"
#include "imgui.h"

namespace GameSmithEditor {

	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_GameViewLayer, GameViewLayer);

	GameSmith::Ref<GameSmith::RenderTexture> GameViewLayer::m_GameScreenTexture = nullptr;

	GameViewLayer::GameViewLayer() : EditorWindow("Game View")
	{
		auto& app = GameSmith::Application::Get();
		auto renderManager = GameSmith::RenderingManager::GetInstance();
		m_GameViewTexture = renderManager->GetRenderAPI()->CreateRenderTexture(app.GetWindow()->GetWidth(), app.GetWindow()->GetHeight());
		GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_GameViewTexture.get()), false);

		m_GameViewHandle = app.GetImGuiInstance()->GenerateTextureSpace(m_GameViewTexture);
	}

	void GameViewLayer::OnImGuiRender()
	{
		if (ImGui::Begin("Game View", &m_Open)) {
			ImGui::Image((ImTextureID)m_GameViewHandle->gpuSpot, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
		}
		ImGui::End();
		
		
	}

	void GameViewLayer::OnUpdate(float dt)
	{
		if (m_GameScreenTexture != nullptr) {
			m_GameScreenTexture->CopyTexture(m_GameViewTexture);
		}
		
		if (!m_Open) {
			CloseWindow();
		}
	}
};

