#pragma once

#ifdef GE_EDITOR_DEBUG
#include "GameSmithEditor/Windows/EditorWindow.h"

namespace GameSmithEditor {
	class PrimitiveCreatorWindow : public EditorWindow
	{
	public:
		PrimitiveCreatorWindow() : EditorWindow("Primitive Creator") {}
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		bool m_Open = true;
	};
}
#endif