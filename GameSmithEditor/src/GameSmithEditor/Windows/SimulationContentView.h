#pragma once
#include "GameSmithEngine.h"
#include "vector"
#include "EditorWindow.h"

namespace GameSmithEditor {
	class SimulationContentView : public EditorWindow {
	public:
		SimulationContentView() : EditorWindow("SimulationContentView") {}
		void OnImGuiRender() override;
		void OnUpdate() override;
		static inline GameSmith::Connection<GameSmith::GameObject> GetCurrentObject() { return m_SelectedObjected; }
	private:
		static GameSmith::Connection<GameSmith::GameObject> m_SelectedObjected;

		int m_Selection = 0;
		std::vector<std::string> m_NamesStd;
	};
};


