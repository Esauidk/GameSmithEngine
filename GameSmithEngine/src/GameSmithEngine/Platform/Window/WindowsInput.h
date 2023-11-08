#pragma once
#include "GameSmithEngine/Core/Input.h"

namespace GameSmith {
	// Windows Implementation of Input Polling System
	class WindowsInput : public Input
	{
	protected:
		bool IsKeyPressedImpl(int keycode) override;
		bool IsMouseButtonPressedImpl(int button) override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
		std::pair<float, float> GetMousePosImpl() override;
	};
};


