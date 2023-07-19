#include "gepch.h"
#include "WindowsWindow.h"
#include "Resource.h"
#include "ProjectGE/Log.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Renderer.h"
#include "imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace ProjectGE {
	WindowsWindow::WindowClass WindowsWindow::WindowClass::wndClass;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowClass::WindowClass() noexcept{
		hInst = GetModuleHandle(nullptr);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetInstance();
		wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();
		wc.hIconSm = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDC_MYICON), IMAGE_ICON, 16, 16, 0));;
		RegisterClassEx(&wc);
	}

	WindowsWindow::WindowClass::~WindowClass() {
		UnregisterClass(wndClassName, GetInstance());
	}

	const wchar_t* WindowsWindow::WindowClass::GetName() noexcept {
		return wndClassName;
	}

	HINSTANCE WindowsWindow::WindowClass::GetInstance() noexcept {
		return wndClass.hInst;
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Prop.Height = props.Height;
		m_Prop.Width = props.Width;
		m_Prop.Title = props.Title;
		m_Prop.renderOption = props.renderOption;

		GE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		//Creating Rect
		RECT wr;
		wr.left = 100;
		wr.right = props.Width + wr.left;
		wr.top = 100;
		wr.bottom = wr.top + props.Height;
		int res = AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		GE_CORE_ASSERT(res, "Could not adjusted RECT when creating window {0}", props.Title);

		std::wstring titleConvert = std::wstring(props.Title.begin(), props.Title.end());

		//Creating Window
		m_HWnd = CreateWindow(
			WindowClass::GetName(),
			titleConvert.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME,
			CW_USEDEFAULT, CW_USEDEFAULT,
			wr.right - wr.left,
			wr.bottom - wr.top,
			nullptr, nullptr,
			WindowClass::GetInstance(),
			this
		);

		GE_CORE_ASSERT(m_HWnd != nullptr, "Could not react window {0}", props.Title);

		switch (props.renderOption) {
		case RenderOptions::DIRECTX12:
		{
			m_RenderContext = (Renderer*)new DirectX12Renderer(m_HWnd);
			break;
		}
		case RenderOptions::NONE:
			GE_CORE_CRITICAL("No render type set");
			m_RenderContext = nullptr;
			break;
		}

		if (m_RenderContext != nullptr) {
			m_RenderContext->Init();
		}
		
		//Show window
		ShowWindow(m_HWnd, SW_SHOWDEFAULT);

		SetVSync(true);
	}

	void WindowsWindow::OnUpdate() {
		ProcessMessages();
		if (m_RenderContext != nullptr) {
			m_RenderContext->Swap();
		}
		
	}

	void WindowsWindow::Shutdown() {
		DestroyWindow(m_HWnd);
		if (m_RenderContext != nullptr) {
			delete m_RenderContext;
		}
		
	}

	void WindowsWindow::SetTitle(const std::string& title) {
		auto titleConvert = std::wstring(title.begin(), title.end());
		int res = SetWindowText(m_HWnd, titleConvert.c_str());
		GE_CORE_ASSERT(res, "Could not change title of window {0}", title);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		m_Prop.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Prop.VSync;
	}

	std::optional<int> WindowsWindow::ProcessMessages() noexcept {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return (int)msg.wParam;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return {};
	}


	LRESULT CALLBACK WindowsWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (msg == WM_NCCREATE) {
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowsWindow* const pWnd = static_cast<WindowsWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::HandleMsgThunk));
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK WindowsWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		WindowsWindow* const pWnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	

	LRESULT WindowsWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
		// Let IMGUI have a crack at it first
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		// Moves on to the rest of our application
		switch (msg) {
		case WM_CLOSE:
		{
			WindowCloseEvent e;
			m_Close.Dispatch(e);
			PostQuitMessage(0);
			return 0;
		}
		case WM_MOVE: 
		{
			WindowMovedEvent e;
			m_Moved.Dispatch(e);
			break;
		}
		case WM_KILLFOCUS:
		{
			WindowLostFocusEvent e;
			m_FocusLost.Dispatch(e);
			break;
		}
		case WM_SETFOCUS:
		{
			WindowFocusEvent e;
			m_Focus.Dispatch(e);
			break;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			auto keyCode = static_cast<unsigned char>(wParam);
			if (lParam & 0x40000000) {
				m_Repeat += LOWORD(lParam);
			}
			else {
				m_Repeat = 0;
			}

			KeyPressedEvent e(keyCode, m_Repeat);
			m_KeyPressed.Dispatch(e);

			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{

			auto keyCode = static_cast<unsigned char>(wParam);
			KeyReleasedEvent e(keyCode);
			m_KeyReleased.Dispatch(e);
			break;
		}
		case WM_CHAR:
		{
			
			auto keyCode = static_cast<unsigned char>(wParam);
			CharEvent e(keyCode);
			m_Char.Dispatch(e);
			break;
		}
		case WM_MOUSEMOVE:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			if (pt.x >= 0 && pt.x < (short)m_Prop.Width && pt.y >= 0 && pt.y < (short)m_Prop.Height) {
				MouseMoveEvent e(pt.x, pt.y);
				m_MouseMove.Dispatch(e);

				// Initiate MouseMove Event
				if (!m_CapturingInput) {
					SetCapture(hWnd);
					m_CapturingInput = true;
				}
			}
			else {
				if (!m_LButtonDown && !m_RButtonDown) {
					ReleaseCapture();
					m_CapturingInput = false;
				}
				
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetForegroundWindow(hWnd);


			m_LButtonDown = true;
			MouseButtonPressEvent e(MouseButtonEvent::LEFT);
			m_MousePressed.Dispatch(e);
			break;
		}
		case WM_RBUTTONDOWN:
		{

			m_RButtonDown = true;
			MouseButtonPressEvent e(MouseButtonEvent::RIGHT);
			m_MousePressed.Dispatch(e);
			break;
		}
		case WM_LBUTTONUP:
		{

			m_LButtonDown = false;
			const POINTS pt = MAKEPOINTS(lParam);
			MouseButtonReleaseEvent e(MouseButtonEvent::LEFT);
			m_MouseReleased.Dispatch(e);
			if (pt.x < 0 || pt.x >= (short)m_Prop.Width || pt.y < 0 || pt.y >= (short)m_Prop.Height) {
				ReleaseCapture();
			}
			break;
		}
		case WM_RBUTTONUP:
		{

			m_RButtonDown = false;
			const POINTS pt = MAKEPOINTS(lParam);
			MouseButtonReleaseEvent e(MouseButtonEvent::RIGHT);
			m_MouseReleased.Dispatch(e);
			if (pt.x < 0 || pt.x >= (short)m_Prop.Width || pt.y < 0 || pt.y >= (short)m_Prop.Height) {
				ReleaseCapture();
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			const float delta = ((float)GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;

			MouseScrollEvent e(delta, pt.x, pt.y);
			m_MouseScroll.Dispatch(e);
			break;
		}
		case WM_SIZE:
		{
			unsigned int width = LOWORD(lParam);
			unsigned int height = HIWORD(lParam);
			m_Prop.Width = width;
			m_Prop.Height = height;
			m_RenderContext->Resize((float)width, (float)height);
			WindowResizeEvent e(width, height);
			m_Resized.Dispatch(e);
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

};

