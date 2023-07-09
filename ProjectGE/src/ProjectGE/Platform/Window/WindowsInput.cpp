#include "gepch.h"
#include "WindowsInput.h"

#include "ProjectGE/Application.h"

namespace ProjectGE {

	Input* Input::m_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		if (GetKeyState(keycode) & 0x8000) {
			return true;
		}

		return false;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		switch (button) {
		case 0:
			if (GetKeyState(VK_LBUTTON) & 0x8000) {
				return true;
			}
			break;
		case 1:
			if (GetKeyState(VK_RBUTTON) & 0x8000) {
				return true;
			}
			break;
		}
		return false;
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return x;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return y;
	}
	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		POINT pt;
		GetCursorPos(&pt);

		auto window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());
		ScreenToClient(window, &pt);

		return { pt.x, pt.y };
	}
};
