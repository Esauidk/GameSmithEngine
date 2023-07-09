#pragma once

#include "ProjectGE/Window.h"
#include "ProjectGE/Rendering/Renderer.h"
#include "ProjectGE/Events/Event.h"

namespace ProjectGE {
	class GE_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		void SetTitle(const std::string& title) override;

		inline unsigned int GetWidth() const override { return m_Prop.Width; }
		inline unsigned int GetHeight() const override { return m_Prop.Height; }
		inline virtual Platform GetPlatform() const override { return Platform::WINDOW; };

		inline void* GetNativeWindow() const override { return m_HWnd; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		static std::optional<int> ProcessMessages() noexcept;
	private:

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			RenderOptions renderOption;

		};

		// This class is required to register with the windows operating system/servers
		class WindowClass {
		public:
			static const wchar_t* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass() noexcept;
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator = (const WindowClass&) = delete;
			static constexpr const wchar_t* wndClassName = L"ProjectGE Window";
			static WindowClass wndClass;
			HINSTANCE hInst;
		};

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		WindowData m_Prop;
		HWND m_HWnd;

		bool m_CapturingInput;
		int m_Repeat;
	};
};


