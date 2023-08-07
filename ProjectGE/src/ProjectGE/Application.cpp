#include "gepch.h"
#include "Application.h"

#include "Log.h"

#include "Input.h"

#include "Rendering/Renderer.h"
#include "Rendering/RenderAgnostics/BasicStructs.h"

namespace ProjectGE {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		GE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Timer = std::make_unique<Timer>();

		ProjectGE::WindowProps props;

		m_Window = std::unique_ptr<Window>(Window::Create(props));

		for (const std::vector<EventDispatcherBase*> dispatchers = m_Window->GetDistpachers(); auto dispatcher : dispatchers) {
			
			bool reg = RegisterEvent<WindowCloseEvent>(dispatcher, GE_BIND_EVENT_FN(Application, OnWindowClose), false);

			if (reg) {
				continue;
			}
		}

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		
		/* START: TEST CODE REMOVE */
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

		m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indexCount, (int)_countof(indexCount)));
		/* END: TEST CODE REMOVE */

	}

	bool Application::OnWindowClose(WindowCloseEvent& evn) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.Push(layer);
		layer->OnAttach();
		layer->EventSubscribe(m_Window->GetDistpachers(), false);
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushSpecial(layer);
		layer->OnAttach();
		layer->EventSubscribe(m_Window->GetDistpachers(), true);
	}

	void Application::Execute() {

		while (m_Running) {
			// TEST CODE: REMOVE LATER
			// TESTING BEGINNING SCENE, BINDING PIPELINE PROPERTIES, SUBMITTING MESH/OBJECT
			Renderer::BeginScene();
			m_Window->GetRenderer()->AttachContextResources();

			m_State->Bind();
			m_Root->Bind();

			m_Topo->Bind();
			Renderer::Submit(m_VertexBuffer.get(), m_IndexBuffer.get());

			Renderer::EndScene();



			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}

	
};
