#include "gepch.h"
#include "ImGuiLayer.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Context.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/Texture/DirectX12Texture.h"
#include "GameSmithEngine/Rendering/DirectX12/Util/third-party/d3dx12.h"

#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

#include "GameSmithEngine/Core/Application.h"
#include "GameSmithEngine/Core/Log.h"
#include "imgui.h"
#include "ImGuiThemes.h"

#define HEAP_INCREASE 5


namespace GameSmith {
	Ref<DirectX12DescriptorHeap> ImGuiLayer::s_Heap  = nullptr;
	unsigned int ImGuiLayer::s_HeapCurSlot = 0;

	static void SetupImGuiStyle()
	{
		ApplyMoonlightTheme();
	}

	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer"), m_DockEnabled(false) {
		s_Heap = DirectX12Core::GetCore().GetHeapDatabase()->AllocateHeap(50, DescriptorHeapType::CBVSRVUAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, "IMGui Heap");
	}

	ImGuiLayer::~ImGuiLayer() {
	}

	void ImGuiLayer::OnAttach(const ApplicationSpecs& specs) {
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

		ImGui_ImplDX12_InitInfo init_info = {};
		init_info.Device = DirectX12Core::GetCore().GetDevice();
		init_info.CommandQueue = DirectX12Core::GetCore().GetDirectCommandContext()->GetQueue().GetCommandQueue();
		init_info.NumFramesInFlight = 2;
		init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;
		init_info.SrvDescriptorHeap = s_Heap->GetHeapReference();
		init_info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) {
			// Get descriptor from end of heap
			*out_cpu_handle = s_Heap->GetCPUReference(s_HeapCurSlot);
			*out_gpu_handle = s_Heap->GetGPUReference(s_HeapCurSlot);
			s_HeapCurSlot++;
		};
		init_info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) {
		};


		ImGui_ImplDX12_Init(&init_info);

		SetupImGuiStyle();
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX12_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
#ifdef GE_DEBUG
		auto& timer = Application::Get().GetTimer();
		ImGui::Begin("Game Stats");
		ImGui::Text("FPS: %f", 1.0f / timer.GetDeltaTimeSeconds());
		ImGui::End();
#endif
	}

	const ImGuiTextureSpace* ImGuiLayer::GenerateTextureSpace(Ref<Texture> tex)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE slot = s_Heap->GetCPUReference(s_HeapCurSlot);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuSlot = s_Heap->GetGPUReference(s_HeapCurSlot);
		auto device = DirectX12Core::GetCore().GetDevice();

		auto d3Tex = CastPtr<DirectX12Texture>(tex);
		auto texRef = d3Tex->GetSRVHandle();
		device->CopyDescriptorsSimple(1, slot, texRef, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		s_HeapCurSlot++;

		Ref<ImGuiTextureSpace> space = Ref<ImGuiTextureSpace>(new ImGuiTextureSpace());
		*space = { (void*)gpuSlot.ptr, s_HeapCurSlot - 1 };
		m_CurrentSpaces.emplace_back(space, texRef);

		return space.get();
	}

	ImGuiContext* ImGuiLayer::GetImGuiContext()
	{
		return ImGui::GetCurrentContext();
	}

	void ImGuiLayer::Begin()
	{	
		if (m_PreviousHeap != nullptr) {
			auto device = DirectX12Core::GetCore().GetDevice();
			while (!m_SpacesToMigrate.empty()) {
				auto& entry = m_SpacesToMigrate.front();
				device->CopyDescriptorsSimple(
					1,
					s_Heap->GetCPUReference(s_HeapCurSlot),
					entry.originalHandle,
					D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
				);

				entry.space->index = s_HeapCurSlot;
				entry.space->gpuSpot = (void*)s_Heap->GetGPUReference(s_HeapCurSlot).ptr;

				s_HeapCurSlot++;
				m_SpacesToMigrate.pop();
			}
			m_PreviousHeap->Free();
			m_PreviousHeap = nullptr;
		}

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (m_DockEnabled) {
			ImGui::DockSpaceOverViewport();
		}
		
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
			s_Heap->GetHeapReference()
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