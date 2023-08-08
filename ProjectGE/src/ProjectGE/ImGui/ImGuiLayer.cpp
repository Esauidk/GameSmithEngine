#include "gepch.h"
#include "ImGuiLayer.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"
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
		const Window& window = app.GetWindow();
		RendererContext* renderer = window.GetRenderer();

		auto hwnd = (HWND)(window.GetNativeWindow());
		ImGui_ImplWin32_Init(hwnd);

		auto* dRender = (DirectX12Context*)renderer;
		auto descriptor = dRender->GetSRVHeap();
		ImGui_ImplDX12_Init(DirectX12Context::GetDevice(),
			2,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			descriptor,
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

	void ImGuiLayer::Begin() const
	{	
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() const
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		const Window& window = app.GetWindow();
		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());
		ImGui::Render();

		RendererContext* renderer = window.GetRenderer();
		auto* dRender = (DirectX12Context*)renderer;
		auto& commandList = DirectX12Context::GetDirectCommandList();

		dRender->AttachContextResources();
		ID3D12DescriptorHeap* descriptorHeaps[] = {
			dRender->GetSRVHeap()
		};
		commandList->SetDescriptorHeaps(1, descriptorHeaps);

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), &commandList);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(nullptr, (void*)(&commandList));
		}

		DirectX12Context::FinalizeCommandList(DirectX12QueueType::Direct);
	}

};