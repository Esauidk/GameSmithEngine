#pragma once
#include "EngineDependenciesOnly.h"

namespace GameSmithEditor {
	class EditorCoreLayer : public GameSmith::Layer
	{
	public:
		EditorCoreLayer();
		void OnImGuiRender() override;
	};
};


