#include "gepch.h"
#include "ViewHandleRenderer.h"
#include <imgui.h>
#include "HandleRegistry.h"

namespace GameSmithEditor {
	void ViewHandleRenderer::Update(float dt)
	{
		m_HandleTypes.clear();
		HandleFactory::GetInstance()->GetRegisteredKeys(&m_HandleTypes);

		if (m_Handle != nullptr && m_TargetScreen != nullptr) {
			m_Handle->DrawHandle(m_TargetScreen);
		}
	}

	void ViewHandleRenderer::DrawHandleMenu()
	{
		for (const auto& handleType : m_HandleTypes) {
			if (ImGui::MenuItem(handleType.c_str())) {
				m_Handle = HandleFactory::GetInstance()->Create(GameSmith::PassKey<ViewHandleRenderer>{}, handleType);
				if (!m_Obj.expired()) {
					m_Handle->SetObject(m_Obj);
				}
			}
		}
	}

	void ViewHandleRenderer::OnImGuiRender()
	{
		if (m_Handle != nullptr) {
			m_Handle->OnImGuiRender();
		}
	}

	void ViewHandleRenderer::SetTargetScreen(GameSmith::Ref<GameSmith::RenderTexture> texture)
	{
		m_TargetScreen = texture;
	}
};