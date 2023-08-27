#include "gepch.h"
#include "ImGuiLayer.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"


#include "ProjectGE/Core/Application.h"
#include "ProjectGE/Core/Log.h"


namespace ProjectGE {
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") {
		m_Heap = DirectX12Core::GetCore().GetDirectCommandContext()->GetHeapDB().AllocateHeap(1, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
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

		auto hwnd = (HWND)(window.GetNativeWindow());
		ImGui_ImplWin32_Init(hwnd);

		ImGui_ImplDX12_Init(DirectX12Core::GetCore().GetDevice(),
			2,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			m_Heap->GetHeapReference(),
			m_Heap->GetCPUReference(0), m_Heap->GetGPUReference(0));
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
		auto context = DirectX12Core::GetCore().GetDirectCommandContext();
		auto& commandList = context->GetCommandList();

		dRender->AttachContextResources();
		ID3D12DescriptorHeap* descriptorHeaps[] = {
			m_Heap->GetHeapReference()
		};
		commandList->SetDescriptorHeaps(1, descriptorHeaps);

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), &commandList);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(nullptr, (void*)(&commandList));
		}

		context->FinalizeCommandList();
	}

};