#pragma once

#include "GameSmithEngine/Core/Window.h"
#include "GameSmithEngine/Rendering/RendererContext.h"
#include "GameSmithEngine/Events/Event.h"

namespace GameSmith {
	// Windows OS implementation of Window abstract class
	// 
	// Look at Window Interface for method documentation
	class GE_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		void SetTitle(const std::string& title) override;

		inline unsigned int GetWidth() const override { return m_Prop.Width; }
		inline unsigned int GetHeight() const override { return m_Prop.Height; }
		inline Platform GetPlatform() const override { return Platform::WINDOW; };

		inline void* GetNativeWindow() const override { return m_HWnd; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void SetFullWindowRender(bool enabled) override;
		virtual bool IsRenderingFullWindow() const override;
		virtual void ChangeRenderLocation(float x, float y, float width, float height) override;

		
	private:
		// Processes Window OS Messages
		static std::optional<int> ProcessMessages() noexcept;

		struct WindowData {
			std::string Title = "Sample Title";
			unsigned int Width = 1920;
			unsigned int Height = 1080;
			bool VSync = true;
			bool m_RenderFullWindow = true;
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
			static constexpr const wchar_t* wndClassName = L"GameSmith Window";
			static WindowClass wndClass;
			HINSTANCE hInst;
		};

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


		virtual void Init(const WindowProps& props);
		virtual void Shutdown();


		WindowData m_Prop;
		HWND m_HWnd;

		bool m_CapturingInput;
		bool m_LButtonDown;
		bool m_RButtonDown;
		int m_Repeat;
	};
};


