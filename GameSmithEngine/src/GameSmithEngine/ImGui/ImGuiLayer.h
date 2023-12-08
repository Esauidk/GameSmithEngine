#pragma once
#include "GameSmithEngine/Core/Layer.h"
#include "GameSmithEngine/Events/ApplicationEvents.h"
#include "GameSmithEngine/Events/KeyboardEvents.h"
#include "GameSmithEngine/Events/MouseEvents.h"

// TEMPORARY
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12HeapDataBase.h"

namespace GameSmith {
	// A layer representing a ImGUI overlay, a GUI on top of the basic window
	class GE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void Begin() const;
		void End() const;
	private:
		Ref<DirectX12DescriptorHeap> m_Heap;
	};
};