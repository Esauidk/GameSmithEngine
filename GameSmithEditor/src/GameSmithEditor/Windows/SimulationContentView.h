#pragma once
#include "GameSmithEditor/EngineDependenciesOnly.h"
#include "vector"

namespace GameSmithEditor {
	class SmulationContentView : public GameSmith::Layer {
	public:
		void OnImGuiRender() override;
		void OnUpdate() override;
	private:
		int m_Selection = 0;
		std::vector<std::string> m_NamesStd;
		std::vector<const char*> m_Objects;

	};
};


