#include "GameViewLayer.h"
#include "imgui.h"

namespace GameSmithEditor {
	GameViewLayer::GameViewLayer() : GameSmith::Layer("Game View")
	{
		auto renderManager = GameSmith::RenderingManager::GetInstance();
		m_RenderTex = renderManager->GetRenderAPI()->CreateRenderTexture((float)GameSmith::Application::Get().GetWindow()->GetWidth(), (float)GameSmith::Application::Get().GetWindow()->GetHeight());

		GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_RenderTex.get()), false);
		renderManager->GetRenderAPI()->SetRenderTexture(m_RenderTex, 0);
		renderManager->SetFrameTexture(m_RenderTex);
	}

	void GameViewLayer::OnImGuiRender()
	{
		ImGui::Begin("Game View");
		ImGui::End();
	}

	void GameViewLayer::OnUpdate()
	{
	}
};

