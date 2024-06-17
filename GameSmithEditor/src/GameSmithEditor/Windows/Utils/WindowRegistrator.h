#pragma once
#include "GameSmithEditor/EditorCoreLayer.h"

namespace GameSmithEditor {
	template<typename T>
	class WindowRegistrator
	{
	public:
		inline WindowRegistrator(std::string window) {
			EditorCoreLayer::RegisterWindow(window, []() {return new T(); });
		}
	};
};


