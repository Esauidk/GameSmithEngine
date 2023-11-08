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
	class Window
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
		
		// Returns the given render context attached to the OS Window
		inline RendererContext* GetRenderer() const { return m_RenderContext; }

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		// Instantiates an implementation of the Window Interface (recommended to use this instead of instantiating a specific implementation)
		static Window* Create(const WindowProps& props = WindowProps());

		// Returns a list of event dispatchers, used when you want to subscribe to a Window event
		inline std::vector<EventDispatcherBase*> GetDistpachers() const { return m_Dispatchers; }

	protected:
		RendererContext* m_RenderContext;

		EventDispatcher<WindowCloseEvent> m_Close;
		EventDispatcher<WindowFocusEvent> m_Focus;
		EventDispatcher<WindowLostFocusEvent> m_FocusLost;
		EventDispatcher<WindowMovedEvent> m_Moved;
		EventDispatcher<WindowResizeEvent> m_Resized;

		EventDispatcher<KeyPressedEvent> m_KeyPressed;
		EventDispatcher<CharEvent> m_Char;
		EventDispatcher<KeyReleasedEvent> m_KeyReleased;

		EventDispatcher<MouseMoveEvent> m_MouseMove;
		EventDispatcher<MouseScrollEvent> m_MouseScroll;
		EventDispatcher<MouseButtonPressEvent> m_MousePressed;
		EventDispatcher<MouseButtonReleaseEvent> m_MouseReleased;

		const std::vector<EventDispatcherBase*> m_Dispatchers = { &m_Close, 
			&m_Focus, &m_FocusLost, 
			&m_Moved, &m_Resized, 
			&m_KeyPressed, &m_Char, &m_KeyReleased, 
			&m_MouseMove, &m_MouseScroll, &m_MousePressed, &m_MouseReleased };
	};


};





