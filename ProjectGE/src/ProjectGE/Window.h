#include "ProjectGE/Core.h"
#include "ProjectGE/Events/Event.h"
#include "ProjectGE/Events/ApplicationEvents.h"
#include "ProjectGE/Events/MouseEvents.h"
#include "ProjectGE/Events/KeyboardEvents.h"
#include "ProjectGE/Rendering/Renderer.h"

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
		RenderOptions renderOption;

		WindowProps(
			const std::string& title = "ProjectGE Engine", 
			unsigned int width = 1280, 
			unsigned int height = 720,
			RenderOptions option = RenderOptions::NONE) 
			: Title(title), Width(width), Height(height), renderOption(option) {}
	};

	class GE_API Window
	{
	public:
		using EventFn = std::function<void(Event&)>;
		virtual ~Window() {};
		
		virtual void OnUpdate() = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;	
		virtual Platform GetPlatform() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

		// Defining dispatcher getter
		inline std::vector<EventDispatcherBase*> GetDistpachers() const { return m_Dispatchers; }

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

		const std::vector<EventDispatcherBase*> m_Dispatchers = { &m_Close, 
			&m_Focus, &m_FocusLost, 
			&m_Moved, &m_Resized, 
			&m_KeyPressed, &m_KeyReleased, 
			&m_MouseMove, &m_MouseScroll, &m_MousePressed, &m_MouseReleased };
	};


};





