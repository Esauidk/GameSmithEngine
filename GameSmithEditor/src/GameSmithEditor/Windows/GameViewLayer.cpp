#include "GameViewLayer.h"
#include "imgui.h"
#include "GameSmithEditor/EditorCoreLayer.h"

namespace GameSmithEditor {

	GETR_REGISTEREDITORWINDOW(GameViewLayer);

	GameSmith::Ref<GameSmith::RenderTexture> GameViewLayer::m_GameScreenTexture = nullptr;

	GameViewLayer::GameViewLayer() : GameSmith::Layer("Game View")
	{
		auto& app = GameSmith::Application::Get();
		auto renderManager = GameSmith::RenderingManager::GetInstance();
		m_GameViewTexture = renderManager->GetRenderAPI()->CreateRenderTexture((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());
		GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_GameViewTexture.get()), false);

		m_GameViewHandle = app.GetImGuiInstance()->GenerateTextureSpace(m_GameViewTexture);
	}

	void GameViewLayer::OnImGuiRender()
	{
		ImGui::Begin("Game View");
		ImGui::Image((ImTextureID)m_GameViewHandle.gpuSpot, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
		ImGui::End();
	}

	void GameViewLayer::OnUpdate()
	{
		if (m_GameScreenTexture != nullptr) {
			m_GameScreenTexture->CopyTexture(m_GameViewTexture);
		}
		
	}
};

