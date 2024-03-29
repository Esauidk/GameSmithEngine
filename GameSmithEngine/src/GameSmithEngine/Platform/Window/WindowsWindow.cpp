#include "gepch.h"
#include "WindowsWindow.h"
#include "resource.h"
#include "imgui.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Context.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GameSmith {

	EventDispatcher<WindowCloseEvent> Window::s_Close;
	EventDispatcher<WindowFocusEvent> Window::s_Focus;
	EventDispatcher<WindowLostFocusEvent> Window::s_FocusLost;
	EventDispatcher<WindowMovedEvent> Window::s_Moved;
	EventDispatcher<WindowResizeEvent> Window::s_Resized;

	EventDispatcher<KeyPressedEvent> Window::s_KeyPressed;
	EventDispatcher<CharEvent> Window::s_Char;
	EventDispatcher<KeyReleasedEvent> Window::s_KeyReleased;

	EventDispatcher<MouseMoveEvent> Window::s_MouseMove;
	EventDispatcher<MouseScrollEvent> Window::s_MouseScroll;
	EventDispatcher<MouseButtonPressEvent> Window::s_MousePressed;
	EventDispatcher<MouseButtonReleaseEvent> Window::s_MouseReleased;

	WindowsWindow::WindowClass WindowsWindow::WindowClass::wndClass;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowClass::WindowClass() noexcept {
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

	WindowsWindow::WindowClass::~WindowClass() noexcept {
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

		GE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		//Creating Rect
		RECT wr = {};
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

		auto renderManager = RenderingManager::GetInstance();
		if (renderManager != nullptr) {
			switch (renderManager->GetAPI()) {
			case RendererAPI::API::DirectX12:
			{
				m_RenderContext = (RendererContext*)new DirectX12Context(m_HWnd, m_Prop.Width, m_Prop.Height);
				break;
			}
			case RendererAPI::API::None:
				GE_CORE_CRITICAL("No render type set");
				m_RenderContext = nullptr;
				break;
			}

			if (m_RenderContext != nullptr) {
				m_RenderContext->Init();
			}
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
		m_RenderContext->SetVsync(enabled);
	}

	bool WindowsWindow::IsVSync() const {
		return m_Prop.VSync;
	}

	void WindowsWindow::SetFullWindowRender(bool enabled)
	{
		m_Prop.m_RenderFullWindow = enabled;
	}

	bool WindowsWindow::IsRenderingFullWindow() const
	{
		return m_Prop.m_RenderFullWindow;
	}

	void WindowsWindow::ChangeRenderLocation(float x, float y, float width, float height)
	{
		if (!m_Prop.m_RenderFullWindow) {
			float resolvedW = MIN(width, m_Prop.Width);
			float resolvedH = MIN(height, m_Prop.Height);
			float resolvedX = MIN(MAX(x, 0), m_Prop.Width - resolvedW);
			float resolvedY = MIN(MAX(y, 0), m_Prop.Height - resolvedH);
			m_RenderContext->ResizeRenderSpace(resolvedX, resolvedY, resolvedW, resolvedH);
		}
		else {
			GE_CORE_INFO("Rendering is set for full window mode, change render setting to render part of the window");
		}
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
			s_Close.Dispatch(e);
			PostQuitMessage(0);
			return 0;
		}
		case WM_MOVE: 
		{
			WindowMovedEvent e;
			s_Moved.Dispatch(e);
			break;
		}
		case WM_KILLFOCUS:
		{
			WindowLostFocusEvent e;
			s_FocusLost.Dispatch(e);
			break;
		}
		case WM_SETFOCUS:
		{
			WindowFocusEvent e;
			s_Focus.Dispatch(e);
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
			s_KeyPressed.Dispatch(e);

			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{

			auto keyCode = static_cast<unsigned char>(wParam);
			KeyReleasedEvent e(keyCode);
			s_KeyReleased.Dispatch(e);
			break;
		}
		case WM_CHAR:
		{
			
			auto keyCode = static_cast<unsigned char>(wParam);
			CharEvent e(keyCode);
			s_Char.Dispatch(e);
			break;
		}
		case WM_MOUSEMOVE:
		{


			const POINTS pt = MAKEPOINTS(lParam);
			if (pt.x >= 0 && pt.x < (short)m_Prop.Width && pt.y >= 0 && pt.y < (short)m_Prop.Height) {
				MouseMoveEvent e(pt.x, pt.y);
				s_MouseMove.Dispatch(e);

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
			s_MousePressed.Dispatch(e);
			break;
		}
		case WM_RBUTTONDOWN:
		{

			m_RButtonDown = true;
			MouseButtonPressEvent e(MouseButtonEvent::RIGHT);
			s_MousePressed.Dispatch(e);
			break;
		}
		case WM_LBUTTONUP:
		{

			m_LButtonDown = false;
			const POINTS pt = MAKEPOINTS(lParam);
			MouseButtonReleaseEvent e(MouseButtonEvent::LEFT);
			s_MouseReleased.Dispatch(e);
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
			s_MouseReleased.Dispatch(e);
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
			s_MouseScroll.Dispatch(e);
			break;
		}
		case WM_SIZE:
		{
			unsigned int width = LOWORD(lParam);
			unsigned int height = HIWORD(lParam);
			m_Prop.Width = width;
			m_Prop.Height = height;

			m_RenderContext->Resize((float)width, (float)height, m_Prop.m_RenderFullWindow);
			
			WindowResizeEvent e(width, height);
			s_Resized.Dispatch(e);
			break;
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

};

