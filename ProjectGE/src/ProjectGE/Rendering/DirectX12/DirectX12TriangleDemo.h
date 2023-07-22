#pragma once

#include "BindableResources/VertexBuffer.h"
#include "BindableResources/IndexBuffer.h"
#include "BindableResources/RootSignature.h"
#include "BindableResources/VertexShader.h"
#include "BindableResources/PixelShader.h"
#include "BindableResources/RootSignature.h"
#include "BindableResources/InputLayout.h"
#include "PipelineState.h"
#include "BindableResources/TopologyResource.h"

namespace ProjectGE {
	class DirectX12TriangleDemo {
	public:
		DirectX12TriangleDemo(ID3D12Device8* device, ID3D12GraphicsCommandList6* list);
		void Draw(ID3D12GraphicsCommandList6* list);
	private:
		struct Vertex {
			float pos[3];
			float color[3];
		};

		std::unique_ptr<PipelineState> m_State;
		std::unique_ptr<InputLayout> m_Layout;
		std::unique_ptr<RootSignature> m_Root;
		std::unique_ptr<TopologyResource> m_Topo;
		std::unique_ptr<VertexShader> m_VS;
		std::unique_ptr<PixelShader> m_PS;
		std::unique_ptr<VertexBuffer<Vertex>> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		int m_IndexCount;
	};
};


