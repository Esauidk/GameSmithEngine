#include "gepch.h"
#include "DirectX12TriangleDemo.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace ProjectGE {

	DirectX12TriangleDemo::DirectX12TriangleDemo()
	{
		// Read Shaders (Vertex, Pixel)
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		auto vertex = std::wstring(buffer).substr(0, pos).append(L"\\/SampleVertexShader.cso");
		auto pixel = std::wstring(buffer).substr(0, pos).append(L"\\/SamplePixelShader.cso");

		std::string nvertex = std::string(vertex.begin(), vertex.end());
		std::string npixel = std::string(pixel.begin(), pixel.end());
		m_VS = std::make_unique<DirectX12VertexShader>(nvertex);
		m_PS = std::make_unique<DirectX12PixelShader>(npixel);

		// Setup Input Layout

		BufferLayoutBuilder layout = { {"POSITION", ShaderDataType::Float3}, {"COLOR", ShaderDataType::Float3} };
		m_Layout = std::make_unique<DirectX12InputLayout>(layout);


		// Setup Root Signature
		m_Root = std::make_unique<DirectX12RootSignature>(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS);
		
		/*CD3DX12_ROOT_PARAMETER1 rootParameters;
		rootParameters.InitAsConstants(sizeof(glm::mat4) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		m_Root->AddParameter(rootParameters);*/

		m_Root->BuildRootSignature();

		// Setup Topology

		m_Topo = std::make_unique<DirectX12TopologyResource>(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Setup Pipeline
		m_State = std::make_unique<DirectX12PipelineState>();
		
		m_VS->Append(*(m_State.get()));

		m_PS->Append(*(m_State.get()));
		m_Layout->Append(*(m_State.get()));
		m_Root->Append(*(m_State.get()));
		m_Topo->Append(*(m_State.get()));

		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_State->Attach(rtvFormats);

		m_State->Attach(DXGI_FORMAT_D32_FLOAT);
		m_State->Build();

		Vertex cubeVertex[] = {
			{ {-0.5f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }, // 0
			{ {0.0f,  0.75f, 0.0f}, {0.0f, 0.0f, 1.0f} }, // 1 
			{ {0.5f,  0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} } // 2
		};

		m_VertexBuffer = std::make_unique<DirectX12VertexBuffer<Vertex>>(cubeVertex, sizeof(cubeVertex) / sizeof(Vertex));

		WORD indexCount[] = {
			0, 1, 2
		};

		m_IndexCount = (int)_countof(indexCount);
		m_IndexBuffer = std::make_unique<DirectX12IndexBuffer>(indexCount, m_IndexCount );
	}	

	void DirectX12TriangleDemo::Draw(Renderer* renderer)
	{
		DirectX12Renderer* dRender = (DirectX12Renderer*)renderer;

		auto list = dRender->GetDrawCommandList();
		//auto list = DirectX12Renderer::GetDirectCommandList();
		m_State->Bind(list.Get());
		m_Root->Bind(list.Get());

		//TODO: Use this as refernece code when doing projection later

		/*glm::mat4 proj = glm::orthoLH(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	
		auto eye = glm::vec3(0, 0, -10);
		auto focus = glm::vec3(0, 0, 0);
		auto up = glm::vec3(0, 1, 0);
		glm::mat4 lookAt = glm::lookAtLH(eye, focus, up);

		auto mvp = proj * lookAt;
		list->SetGraphicsRoot32BitConstants(0, sizeof(glm::mat4) / 4, &mvp, 0);*/

		m_Topo->Bind(list.Get());
		m_VertexBuffer->Bind(list.Get());
		m_IndexBuffer->Bind(list.Get());

		list->DrawIndexedInstanced(m_IndexCount, 1, 0, 0, 0);

		DirectX12Renderer::AsyncJobSubmission(list, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	
};
