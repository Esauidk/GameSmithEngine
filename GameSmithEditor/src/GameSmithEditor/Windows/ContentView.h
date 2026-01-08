#pragma once
#include "EditorWindow.h"
#include "GameSmithEngine.h"
#include <vector>

namespace GameSmithEditor {
	class ContentView : public EditorWindow {
	public:
		ContentView() : EditorWindow("ContentView") {}
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		int m_Selection = 0;
		std::vector<std::string> m_NamesStd;
	};
};


