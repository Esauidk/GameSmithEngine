#pragma once
#include "ViewHandle.h"
#include <vector>
#include <string>

namespace GameSmithEditor {
	class ViewHandleRenderer {
	public:
		ViewHandleRenderer() = default;
		void Update(float dt);
		void DrawHandleMenu();
		void OnImGuiRender();
		void SetTargetScreen(GameSmith::Ref<GameSmith::RenderTexture> texture);
	private:
		std::vector<std::string> m_HandleTypes;
		GameSmith::Connection<GameSmith::GameObject> m_Obj;
		GameSmith::Ref<IViewHandle> m_Handle;
		GameSmith::Ref<GameSmith::RenderTexture> m_TargetScreen;
	};
};