#pragma once
#include "ViewHandle.h"
#include "GameSmithEngine.h"

namespace GameSmithEditor {
	class SampleHandle : public IViewHandle {
	public:
		void SetObject(GameSmith::Connection<GameSmith::GameObject> obj) override {
			GE_APP_INFO("SampleHandle: Object set.");
		}
		void DrawHandle(GameSmith::Ref<GameSmith::RenderTexture> texture) override {
			GE_APP_INFO("SampleHandle: Drawing handle.");
		}
		void OnImGuiRender() override {
			GE_APP_INFO("SampleHandle: Rendering ImGui.");
		}
	};
};