#include "gepch.h"
#include "WindowsWindow.h"
#include "Resource.h"
#include "ProjectGE/Log.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Renderer.h"

namespace ProjectGE {
	WindowsWindow::WindowClass WindowsWindow::WindowClass::wndClass;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
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

		GE_CORE_ASSERT(m_HWnd == nullptr, "Could not react window {0}", props.Title);

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
			//m_RenderContext->EndFrame();
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
		std::wstring titleConvert = std::wstring(title.begin(), title.end());
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
		
		switch (msg) {
		case WM_CLOSE:
		{
			// TODO: Do WindowQuitEvent Here
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
			//kbd.ClearState();
			// TODO: Create FocusLostEvent Here
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
			unsigned char keyCode = static_cast<unsigned char>(wParam);
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

			unsigned char keyCode = static_cast<unsigned char>(wParam);
			KeyReleasedEvent e(keyCode);
			m_KeyReleased.Dispatch(e);
			//kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			//TODO: Create KeyReleasedEvent Here
			break;
		}
		case WM_CHAR:
		{
			
			unsigned char keyCode = static_cast<unsigned char>(wParam);
			CharEvent e(keyCode);
			m_Char.Dispatch(e);
			// kbd.OnChar(static_cast<unsigned char>(wParam));
			// TODO: Create KeyPressedEvent with repeat on
			break;
		}
		case WM_MOUSEMOVE:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			if (pt.x >= 0 && pt.x < m_Prop.Width && pt.y >= 0 && pt.y < m_Prop.Height) {
				//mouse.OnMouseMove(pt.x, pt.y);
				MouseMoveEvent e(pt.x, pt.y);
				m_MouseMove.Dispatch(e);

				// Initiate MouseMove Event
				if (!m_CapturingInput) {
					SetCapture(hWnd);
					m_CapturingInput = true;
				}
			}
			else {
				ReleaseCapture();
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetForegroundWindow(hWnd);



			const POINTS pt = MAKEPOINTS(lParam);
			// mouse.OnLeftPressed(pt.x, pt.y);
			MouseButtonPressEvent e(MouseButtonEvent::LEFT);
			m_MousePressed.Dispatch(e);
			break;
		}
		case WM_RBUTTONDOWN:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			//mouse.OnRightPressed(pt.x, pt.y);
			MouseButtonPressEvent e(MouseButtonEvent::RIGHT);
			m_MousePressed.Dispatch(e);
			break;
		}
		case WM_LBUTTONUP:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			// mouse.OnLeftReleased(pt.x, pt.y);
			MouseButtonReleaseEvent e(MouseButtonEvent::LEFT);
			m_MouseReleased.Dispatch(e);
			if (pt.x < 0 || pt.x >= m_Prop.Width || pt.y < 0 || pt.y >= m_Prop.Height) {
				ReleaseCapture();
				//mouse.OnMouseLeave();
			}
			break;
		}
		case WM_RBUTTONUP:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			// mouse.OnRightReleased(pt.x, pt.y);
			MouseButtonReleaseEvent e(MouseButtonEvent::RIGHT);
			m_MouseReleased.Dispatch(e);
			// TODO: Create MouseButtonReleasedEvent
			if (pt.x < 0 || pt.x >= m_Prop.Width || pt.y < 0 || pt.y >= m_Prop.Height) {
				ReleaseCapture();
				//mouse.OnMouseLeave();
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			const int xPos = LOWORD(lParam);
			const int yPos = HIWORD(lParam);

			MouseScrollEvent e(delta, xPos, yPos);
			m_MouseScroll.Dispatch(e);

			//mouse.OnWheelDelta(pt.x, pt.y, delta);
			// TODO: Create MouseScrollEvent
			break;
		}
		case WM_SIZE:
		{
			unsigned int width = LOWORD(lParam);
			unsigned int height = HIWORD(lParam);
			m_Prop.Width = width;
			m_Prop.Height = height;
			m_RenderContext->Resize(width, height);
			WindowResizeEvent e(width, height);
			m_Resized.Dispatch(e);
		}

		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

};

