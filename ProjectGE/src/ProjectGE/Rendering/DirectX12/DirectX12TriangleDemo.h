#pragma once

#include "BindableResources/DirectX12VertexBuffer.h"
#include "BindableResources/DirectX12IndexBuffer.h"
#include "BindableResources/DirectX12RootSignature.h"
#include "BindableResources/DirectX12VertexShader.h"
#include "BindableResources/DirectX12PixelShader.h"
#include "BindableResources/DirectX12RootSignature.h"
#include "BindableResources/DirectX12InputLayout.h"
#include "DirectX12PipelineState.h"
#include "BindableResources/DirectX12TopologyResource.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

#include "ProjectGE/Rendering/RenderAgnostics/TriangleDemo.h"

namespace ProjectGE {
	class DirectX12TriangleDemo : public TriangleDemo{
	public:
		DirectX12TriangleDemo();
		void Draw(Renderer* renderer);
	private:
		std::unique_ptr<DirectX12PipelineState> m_State;
		std::unique_ptr<DirectX12InputLayout> m_Layout;
		std::unique_ptr<DirectX12RootSignature> m_Root;
		std::unique_ptr<DirectX12TopologyResource> m_Topo;
		std::unique_ptr<DirectX12VertexShader> m_VS;
		std::unique_ptr<DirectX12PixelShader> m_PS;
		std::unique_ptr<DirectX12VertexBuffer<Vertex>> m_VertexBuffer;
		std::unique_ptr<DirectX12IndexBuffer> m_IndexBuffer;

		int m_IndexCount;
	};
};


