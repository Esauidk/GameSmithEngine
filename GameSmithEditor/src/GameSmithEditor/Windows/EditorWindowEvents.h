#pragma once
#include <GameSmithEngine.h>

namespace GameSmithEditor {
	// Forward declaration
	class EditorWindow;

	class EditorWindowClose : public GameSmith::Event {
	public:
		EditorWindowClose(EditorWindow* window) : m_ClosingWindow(window) {
		}

		EVENT_TYPE(MISCELLENIOUS)
	private:
		CATEGORY_TYPE(GameSmith::EventCategory::APP_EVENT)
	private:
		EditorWindow* m_ClosingWindow;
	};
};
