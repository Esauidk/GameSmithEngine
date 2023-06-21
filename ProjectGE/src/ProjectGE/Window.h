#include "ProjectGE/Core.h"
#include "ProjectGE/Events/Event.h"
#include "ProjectGE/Events/ApplicationEvents.h"
#include "ProjectGE/Events/MouseEvents.h"
#include "ProjectGE/Events/KeyboardEvents.h"

namespace ProjectGE {

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
			const std::string& title = "ProjectGE Engine", 
			unsigned int width = 1280, 
			unsigned int height = 720) 
			: Title(title), Width(width), Height(height) {}
	};

	class GE_API Window
	{
	public:
		using EventFn = std::function<void(Event&)>;
		virtual ~Window() {};
		
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;	
		virtual Platform GetPlatform() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

		// Defining dispatcher getters
		inline EventDispatcher<WindowCloseEvent>& GetDispatcherClose() { return m_Close; };
		inline EventDispatcher<WindowFocusEvent>& GetDispatcherFocus() { return m_Focus; };
		inline EventDispatcher<WindowLostFocusEvent>& GetDispatcherFocusLost() { return m_FocusLost; };
		inline EventDispatcher<WindowMovedEvent>& GetDispatcherMove() { return m_Moved; }
		inline EventDispatcher<WindowResizeEvent>& GetDispatcherResize() { return m_Resized; }

		inline EventDispatcher<KeyPressedEvent>& GetDispatcherKeyPress() { return m_KeyPressed; }
		inline EventDispatcher<KeyReleasedEvent>& GetDispatcherKeyRelease() { return m_KeyReleased; }

		inline EventDispatcher<MouseMoveEvent>& GetDispatcherMouseMove() { return m_MouseMove; }
		inline EventDispatcher<MouseScrollEvent>& GetDispatcherMouseScroll() { return m_MouseScroll; }
		inline EventDispatcher<MouseButtonPressEvent>& GetDispatcherMousePress() { return m_MousePressed; }
		inline EventDispatcher<MouseButtonReleaseEvent>& GetDispatcherMouseRelease() { return m_MouseReleased; }

	protected:
		EventDispatcher<WindowCloseEvent> m_Close;
		EventDispatcher<WindowFocusEvent> m_Focus;
		EventDispatcher<WindowLostFocusEvent> m_FocusLost;
		EventDispatcher<WindowMovedEvent> m_Moved;
		EventDispatcher<WindowResizeEvent> m_Resized;

		EventDispatcher<KeyPressedEvent> m_KeyPressed;
		EventDispatcher<KeyReleasedEvent> m_KeyReleased;

		EventDispatcher<MouseMoveEvent> m_MouseMove;
		EventDispatcher<MouseScrollEvent> m_MouseScroll;
		EventDispatcher<MouseButtonPressEvent> m_MousePressed;
		EventDispatcher<MouseButtonReleaseEvent> m_MouseReleased;
	};


};





