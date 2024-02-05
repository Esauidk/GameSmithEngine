#include "gepch.h"
#include "ImGuiLayer.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Context.h"
#include "GameSmithEngine/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

#include "GameSmithEngine/Core/Application.h"
#include "GameSmithEngine/Core/Log.h"


namespace GameSmith {
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer"), m_CurSlot(1) {
		m_Heap = DirectX12Core::GetCore().GetHeapDatabase()->AllocateHeap(2, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, "IMGui Heap");
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
		const Window* window = app.GetWindow();

		auto hwnd = (HWND)(window->GetNativeWindow());
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
		//ImGui::ShowDemoWindow(&show);
	}

	D3D12_GPU_DESCRIPTOR_HANDLE ImGuiLayer::GenerateTextureSpace(D3D12_CPU_DESCRIPTOR_HANDLE tex)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE slot = m_Heap->GetCPUReference(m_CurSlot);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuSlot = m_Heap->GetGPUReference(m_CurSlot);
		auto device = DirectX12Core::GetCore().GetDevice();
		device->CopyDescriptorsSimple(1, slot, tex, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		m_CurSlot++;
		return gpuSlot;
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
		const Window* window = app.GetWindow();
		io.DisplaySize = ImVec2((float)window->GetWidth(), (float)window->GetHeight());
		ImGui::Render();

		auto context = DirectX12Core::GetCore().GetDirectCommandContext();
		auto& commandList = context->GetCommandList();
		context->GetStateManager().BindRenderTargetsOnly();

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