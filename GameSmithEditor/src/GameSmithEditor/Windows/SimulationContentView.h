#pragma once
#include "GameSmithEditor/EngineDependenciesOnly.h"
#include "vector"
#include "GameSmithEditor/Windows/Utils/WindowRegistrator.h"

namespace GameSmithEditor {
	class SimulationContentView : public GameSmith::Layer {
	public:
		SimulationContentView() : GameSmith::Layer("SimulationContentView") {}
		void OnImGuiRender() override;
		void OnUpdate() override;
		static inline GameSmith::Connection<GameSmith::GameObject> GetCurrentObject() { return m_SelectedObjected; }
	private:
		static GameSmith::Connection<GameSmith::GameObject> m_SelectedObjected;
		static WindowRegistrator<SimulationContentView> s_Registrator;

		int m_Selection = 0;
		std::vector<std::string> m_NamesStd;
	};
};


