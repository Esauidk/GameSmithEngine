#include "gepch.h"
#include "TriangleDemo.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/DirectX12TriangleDemo.h"
#endif

namespace ProjectGE {
	ProjectGE::TriangleDemo::TriangleDemo()
	{
		// Read Shaders(Vertex, Pixel)
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		auto vertex = std::wstring(buffer).substr(0, pos).append(L"\\/SampleVertexShader.cso");
		auto pixel = std::wstring(buffer).substr(0, pos).append(L"\\/SamplePixelShader.cso");

		std::string nvertex = std::string(vertex.begin(), vertex.end());
		std::string npixel = std::string(pixel.begin(), pixel.end());
		m_VS = std::unique_ptr<Shader>(Shader::Create(nvertex, ShaderType::Vertex));
		m_PS = std::unique_ptr<Shader>(Shader::Create(npixel, ShaderType::Pixel));

		// Setup Root Signature
		m_Root = std::unique_ptr<ShaderArguementDefiner>(ShaderArguementDefiner::Create());
		m_Root->AddArguement(sizeof(glm::mat4) / 4, ShaderArguementType::Constant);

		/*CD3DX12_ROOT_PARAMETER1 rootParameters;
		rootParameters.InitAsConstants(sizeof(glm::mat4) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		m_Root->AddParameter(rootParameters);*/

		m_Root->FinalizeSignature();

		// Setup Topology

		m_Topo = std::unique_ptr<Topology>(Topology::Create(TopologyType::Triangle));

		// Setup Pipeline
		m_State = std::unique_ptr<PipelineStateObject>(PipelineStateObject::Create());

		m_VS->Append(*(m_State.get()));

		m_PS->Append(*(m_State.get()));
		m_Root->Append(*(m_State.get()));
		m_Topo->Append(*(m_State.get()));

		Vertex cubeVertex[] = {
			{ {-0.5f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }, // 0
			{ {0.0f,  0.75f, 0.0f}, {0.0f, 0.0f, 1.0f} }, // 1 
			{ {0.5f,  0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} } // 2
		};

		BufferLayoutBuilder layout = { {"POSITION", ShaderDataType::Float3}, {"COLOR", ShaderDataType::Float3} };

		m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(&cubeVertex, sizeof(cubeVertex) / sizeof(Vertex)));
		m_VertexBuffer->AttachLayout(layout);
		auto configuredLayout = m_VertexBuffer->GetLayout();
		configuredLayout->Append(*(m_State.get()));

		m_State->Build();

		WORD indexCount[] = {
			0, 1, 2
		};

		m_IndexCount = (int)_countof(indexCount);
		m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indexCount, m_IndexCount));
	}
	TriangleDemo* TriangleDemo::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::DirectX12:
			return new DirectX12TriangleDemo();
		}
		return nullptr;
	}
};

