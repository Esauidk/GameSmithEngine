#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Events/Event.h"
#include "GameSmithEngine/Events/ApplicationEvents.h"
#include "GameSmithEngine/Events/MouseEvents.h"
#include "GameSmithEngine/Events/KeyboardEvents.h"
#include "GameSmithEngine/Rendering/RendererContext.h"

namespace GameSmith {

	enum class Platform {
		WINDOW,
		MAC,
		LINUX
	};

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(
			const std::string& title = "GameSmith Engine", 
			unsigned int width = 1280, 
			unsigned int height = 720) 
			: Title(title), Width(width), Height(height) {}
	};

	// INTERFACE
	// An abstract representation of a OS Window. This holds the responsibilitie of rendering a window to the screen
	// and sending OS events relating to the Window.
	class GE_API Window
	{
	public:
		using EventFn = std::function<void(Event&)>;
		virtual ~Window() = default;
		
		// Any behavior a window needs to occur in a time-cycle (ex: every frame)
		virtual void OnUpdate() = 0;
		// Changes the title of current OS Window instance this represents
		virtual void SetTitle(const std::string& title) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;	
		virtual Platform GetPlatform() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		
		virtual void SetFullWindowRender(bool enabled) = 0;
		virtual bool IsRenderingFullWindow() const = 0;
		virtual void ChangeRenderLocation(float x, float y, float width, float height) = 0;

		virtual void* GetNativeWindow() const = 0;

		// Instantiates an implementation of the Window Interface (recommended to use this instead of instantiating a specific implementation)
		static Window* Create(const WindowProps& props = WindowProps());

	public:
		// Event dispatchers that all windows can emit
		static EventDispatcher<WindowCloseEvent> s_Close;
		static EventDispatcher<WindowFocusEvent> s_Focus;
		static EventDispatcher<WindowLostFocusEvent> s_FocusLost;
		static EventDispatcher<WindowMovedEvent> s_Moved;
		static EventDispatcher<WindowResizeEvent> s_Resized;

		static EventDispatcher<KeyPressedEvent> s_KeyPressed;
		static EventDispatcher<CharEvent> s_Char;
		static EventDispatcher<KeyReleasedEvent> s_KeyReleased;

		static EventDispatcher<MouseMoveEvent> s_MouseMove;
		static EventDispatcher<MouseScrollEvent> s_MouseScroll;
		static EventDispatcher<MouseButtonPressEvent> s_MousePressed;
		static EventDispatcher<MouseButtonReleaseEvent> s_MouseReleased;
	protected:
		RendererContext* m_RenderContext = nullptr;
	};


};





