#pragma once
#include "GameSmithEngine/Core/Layer.h"
#include "GameSmithEngine/Events/ApplicationEvents.h"
#include "GameSmithEngine/Events/KeyboardEvents.h"
#include "GameSmithEngine/Events/MouseEvents.h"

#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12HeapDataBase.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"

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
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		ImGuiTextureSpace GenerateTextureSpace(Ref<RenderTexture> tex);
		inline void SetDockspace(bool enabled) { m_DockEnabled = enabled; };
		void Begin() const;
		void End() const;
	private:
		Ref<DirectX12DescriptorHeap> m_Heap;
		unsigned int m_CurSlot;
		bool m_DockEnabled;
	};
};
