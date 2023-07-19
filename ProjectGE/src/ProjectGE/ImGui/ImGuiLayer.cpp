#include "gepch.h"
#include "ImGuiLayer.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Renderer.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "Backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"


#include "ProjectGE/Application.h"
#include "ProjectGE/Log.h"


namespace ProjectGE {
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") {
		
	}

	ImGuiLayer::~ImGuiLayer() {
	}

	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		Window& window = app.GetWindow();
		Renderer* renderer = window.GetRenderer();

		HWND hwnd = static_cast<HWND>(window.GetNativeWindow());
		ImGui_ImplWin32_Init(hwnd);

		DirectX12Renderer* dRender = (DirectX12Renderer*)renderer;
		auto descriptor = dRender->GetSRVHeap();
		ImGui_ImplDX12_Init(dRender->GetDevice().Get(),
			2,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			descriptor.Get(),
			descriptor->GetCPUDescriptorHandleForHeapStart(), descriptor->GetGPUDescriptorHandleForHeapStart());
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX12_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::Begin()
	{	
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());
		ImGui::Render();

		Renderer* renderer = window.GetRenderer();
		DirectX12Renderer* dRender = (DirectX12Renderer*)renderer;
		auto queue = dRender->GetCommandQueue();
		auto commandList = queue.GetCommandList();
		commandList->SetDescriptorHeaps(1, dRender->GetSRVHeap().GetAddressOf());
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv = dRender->GetRenderTarget();
		commandList->OMSetRenderTargets(1, &rtv, FALSE, NULL);

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
		UINT value = queue.ExecuteCommandList(commandList);
		queue.WaitForFenceValue(value);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(nullptr, (void*)(commandList.Get()));
		}

	}

};