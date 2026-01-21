#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Layer.h"
#include "GameSmithEngine/Events/ApplicationEvents.h"
#include "GameSmithEngine/Events/KeyboardEvents.h"
#include "GameSmithEngine/Events/MouseEvents.h"

#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12HeapDataBase.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"

struct ImGuiContext;
namespace GameSmith {
	struct ImGuiTextureSpace {
		void* gpuSpot;
		unsigned int index;
	};

	// A layer representing a ImGUI overlay, a GUI on top of the basic window
	class GE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach(const ApplicationSpecs& specs) override;
		void OnDetach() override;
		void OnImGuiRender() override;

		const ImGuiTextureSpace* GenerateTextureSpace(Ref<Texture> tex);
		inline void SetDockspace(bool enabled) { m_DockEnabled = enabled; };

		ImGuiContext* GetImGuiContext();
		void Begin();
		void End() const;
	private:
		struct SpaceEntry {
			Ref<ImGuiTextureSpace> space;
			D3D12_CPU_DESCRIPTOR_HANDLE originalHandle;
		};
	private:
		static Ref<DirectX12DescriptorHeap> s_Heap;
		static unsigned int s_HeapCurSlot;
		std::vector<SpaceEntry> m_CurrentSpaces;

		Ref<DirectX12DescriptorHeap> m_PreviousHeap;
		std::queue<SpaceEntry> m_SpacesToMigrate;
		bool m_DockEnabled;
	};
};
