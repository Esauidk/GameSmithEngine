#include "gepch.h"
#include "ImGuiLayer.h"

#include "ProjectGE/Application.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Renderer.h"
#include "DirectX12/imgui_impl_dx12.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

#include "ProjectGE/Log.h"

namespace ProjectGE {
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY
		io.KeyMap[ImGuiKey_Tab] = 0x09;
		io.KeyMap[ImGuiKey_A] = 0x41;
		io.KeyMap[ImGuiKey_C] = 0x43;

		Renderer* renderer = Application::Get().GetWindow().GetRenderer();
		DirectX12Renderer* dRender = (DirectX12Renderer*)renderer;
		auto descriptor = dRender->GetSRVHeap();
		ImGui_ImplDX12_Init(dRender->GetDevice().Get(),
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			descriptor.Get(), 
			descriptor->GetCPUDescriptorHandleForHeapStart(), descriptor->GetGPUDescriptorHandleForHeapStart());
	}

	ImGuiLayer::~ImGuiLayer() {
	}

	void ImGuiLayer::OnAttach() {
	
	}

	void ImGuiLayer::OnDetach() {
	
	}

	void ImGuiLayer::OnUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());
		io.DeltaTime = app.GetTimer().Mark();

		ImGui_ImplDX12_NewFrame();
		ImGui::NewFrame();
		float time;
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();

		// DirectX Setup Code
		Renderer* renderer = Application::Get().GetWindow().GetRenderer();
		DirectX12Renderer* dRender = (DirectX12Renderer*)renderer;
		auto queue = dRender->GetCommandQueue();
		auto commandList = queue.GetCommandList();
		commandList->SetDescriptorHeaps(1, dRender->GetSRVHeap().GetAddressOf());
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = dRender->GetRenderTarget();
		commandList->OMSetRenderTargets(1, &rtv, FALSE, NULL);

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
		UINT value = queue.ExecuteCommandList(commandList);
		queue.WaitForFenceValue(value);
	
	}

	void ImGuiLayer::EventSubscribe(std::vector<EventDispatcherBase*> dispatchers, bool overlay) {
		for (auto dispatcher : dispatchers) {
			bool reg = RegisterEvent<MouseButtonPressEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnMouseButtonPressedEvent), false);
			if (reg) {
				continue;
			}

			reg = RegisterEvent<MouseButtonReleaseEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnMouseButtonReleasedEvent), false);
			if (reg) {
				continue;
			}

			reg = RegisterEvent<MouseMoveEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnMouseMoveEvent), false);
			if (reg) {
				continue;
			}

			reg = RegisterEvent<MouseScrollEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnMouseScrollEvent), false);
			if (reg) {
				continue;
			}

			reg = RegisterEvent<KeyPressedEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnKeyPressedEvent), false);
			if (reg) {
				continue;
			}

			reg = RegisterEvent<CharEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnCharEvent), false);
			if (reg) {
				continue;
			}

			reg = RegisterEvent<KeyReleasedEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnKeyReleasedEvent), false);
			if (reg) {
				continue;
			}

			reg = RegisterEvent<WindowResizeEvent>(dispatcher, GE_BIND_EVENT_FN(ImGuiLayer, OnWindowResizeEvent), false);
			if (reg) {
				continue;
			}
		}
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleaseEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButton()] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& e)
	{
		GE_CORE_INFO(e);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel = e.GetDelta();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		// TODO: Add for special modifiers (Cntrl, Shift, Alt, Super)
		return false;
	}

	bool ImGuiLayer::OnCharEvent(CharEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int c = e.GetKeyCode();
		if (c > 0 && c < 0x10000) {
			io.AddInputCharacter(c);
		}
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		// TODO: Come back to this
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		return false;
	}

};