#pragma once
#include "GameSmithEngine.h"

namespace GameSmithEditor {
	class IViewHandle {
	public:
		virtual void SetObject(GameSmith::Connection<GameSmith::GameObject> obj) = 0;
		virtual void DrawHandle(GameSmith::Ref<GameSmith::RenderTexture> texture) = 0;
		virtual void OnImGuiRender() = 0;
	};
};