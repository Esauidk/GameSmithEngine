#pragma once
#include "GameSmithEditor/EngineDependenciesOnly.h"
#include "vector"

namespace GameSmithEditor {
	class SimulationContentView : public GameSmith::Layer {
	public:
		SimulationContentView() : GameSmith::Layer("SimulationContentView") {}
		void OnImGuiRender() override;
		void OnUpdate() override;
		static inline GameSmith::Connection<GameSmith::GameObject> GetCurrentObject() { return m_SelectedObjected; }
	private:
		static GameSmith::Connection<GameSmith::GameObject> m_SelectedObjected;

		int m_Selection = 0;
		std::vector<std::string> m_NamesStd;
	};
};


