#include <ProjectGE.h>

#include "imgui.h"

class ExampleLayer : public ProjectGE::Layer {
public:

	ExampleLayer() : Layer("Example"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f) {
		/* START: TEST CODE REMOVE */
		// Read Shaders(Vertex, Pixel)

		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		auto vertex = std::wstring(buffer).substr(0, pos).append(L"\\/SampleVertexShader.cso");
		auto pixel = std::wstring(buffer).substr(0, pos).append(L"\\/SamplePixelShader.cso");

		std::string nvertex = std::string(vertex.begin(), vertex.end());
		std::string npixel = std::string(pixel.begin(), pixel.end());
		m_VS = std::unique_ptr<ProjectGE::Shader>(ProjectGE::Shader::Create(nvertex, ProjectGE::ShaderType::Vertex));
		m_PS = std::unique_ptr<ProjectGE::Shader>(ProjectGE::Shader::Create(npixel, ProjectGE::ShaderType::Pixel));

		// Setup Root Signature
		m_Root = std::unique_ptr<ProjectGE::ShaderArguementDefiner>(ProjectGE::ShaderArguementDefiner::Create());

		m_Arg1 = std::unique_ptr<ProjectGE::ShaderArguement>(m_Root->AddArguement(sizeof(glm::mat4) / 4, ProjectGE::ShaderArguementType::Constant));


		m_Root->FinalizeSignature();

		// Setup Topology

		auto m_Topo = std::shared_ptr<ProjectGE::Topology>(ProjectGE::Topology::Create(ProjectGE::TopologyType::Triangle));

		// Setup Pipeline
		m_State = std::unique_ptr<ProjectGE::PipelineStateObject>(ProjectGE::PipelineStateObject::Create());

		m_VS->Append(*(m_State.get()));

		m_PS->Append(*(m_State.get()));
		m_Root->Append(*(m_State.get()));
		m_Topo->Append(*(m_State.get()));

		ProjectGE::Vertex triVertex[] = {
			{ {-0.5f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }, // 0
			{ {0.0f,  0.75f, 0.0f}, {0.0f, 0.0f, 1.0f} }, // 1 
			{ {0.5f,  0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} } // 2
		};

		ProjectGE::Vertex squareVertex[] = {
			{{-0.75f, -0.75f, 0.0f}, {1.0f, 1.0f, 0.0f}},
			{{0.75f, -0.75f, 0.0f}, {0.0f, 1.0f, 1.0f}},
			{{0.75f,  0.75f, 0.0f}, {1.0, 0.0f, 1.0f}},
			{{-0.75f,  0.75f, 0.0f}, {0.5f, 0.5f, 0.5f}}
		};

		ProjectGE::BufferLayoutBuilder layout = { {"POSITION", ProjectGE::ShaderDataType::Float3}, {"COLOR", ProjectGE::ShaderDataType::Float3} };

		auto m_TriVertexBuffer = std::shared_ptr<ProjectGE::VertexBuffer>(ProjectGE::VertexBuffer::Create(&triVertex, sizeof(triVertex) / sizeof(ProjectGE::Vertex)));
		auto m_SquareVertexBuffer = std::shared_ptr<ProjectGE::VertexBuffer>(ProjectGE::VertexBuffer::Create(&squareVertex, sizeof(squareVertex) / sizeof(ProjectGE::Vertex)));
		m_TriVertexBuffer->AttachLayout(layout);
		m_SquareVertexBuffer->AttachLayout(layout);

		auto configuredLayout = m_TriVertexBuffer->GetLayout();
		configuredLayout->Append(*(m_State.get()));

		m_State->Build();

		WORD indexCount[] = {
			0, 1, 2
		};

		WORD squareIndex[] = {
			2,1,0,0,3,2
		};

		auto m_TriIndexBuffer = std::shared_ptr<ProjectGE::IndexBuffer>(ProjectGE::IndexBuffer::Create(indexCount, (int)_countof(indexCount)));
		auto m_SquareIndexBuffer = std::shared_ptr<ProjectGE::IndexBuffer>(ProjectGE::IndexBuffer::Create(squareIndex, (int)_countof(squareIndex)));

		m_TriPack = std::unique_ptr<ProjectGE::GeometryPack>(ProjectGE::GeometryPack::Create());
		m_TriPack->AttachVertexBuffer(m_TriVertexBuffer);
		m_TriPack->AttachIndexBuffer(m_TriIndexBuffer);
		m_TriPack->AttachTopology(m_Topo);

		m_SquarePack = std::unique_ptr<ProjectGE::GeometryPack>(ProjectGE::GeometryPack::Create());
		m_SquarePack->AttachVertexBuffer(m_SquareVertexBuffer);
		m_SquarePack->AttachIndexBuffer(m_SquareIndexBuffer);
		m_SquarePack->AttachTopology(m_Topo);

		/* END: TEST CODE REMOVE */
	}

	void OnImGuiRender() override {}


	void OnUpdate() override {
		float dt = ProjectGE::Application::Get().GetTimer().GetDeltaTimeSeconds();
		glm::vec3 oldPos = m_Cam.GetPosition();
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_A)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(-1, 0, 0);
			m_Cam.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_D)) {
			glm::vec3 newPosition = oldPos + dt * glm::vec3(1, 0, 0);
			m_Cam.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_W)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(0, 1, 0);
			m_Cam.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_S)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(0, -1, 0);
			m_Cam.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_R)) {
			float oldRotation = m_Cam.GetRotation();
			m_Cam.SetRotation(oldRotation + dt * 1);
		}

		ProjectGE::Renderer::BeginScene(m_Cam);
		ProjectGE::Application::Get().GetWindow().GetRenderer()->AttachContextResources();

		m_State->Bind();
		m_Root->Bind();

		ProjectGE::Renderer::Submit(m_TriPack.get(), m_Arg1.get());
		ProjectGE::Renderer::Submit(m_SquarePack.get(), m_Arg1.get());

		ProjectGE::Renderer::EndScene();
	}

	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {

	}

private:
	std::unique_ptr<ProjectGE::PipelineStateObject> m_State;
	std::unique_ptr<ProjectGE::ShaderArguementDefiner> m_Root;
	std::unique_ptr<ProjectGE::Shader> m_VS;
	std::unique_ptr<ProjectGE::Shader> m_PS;
	std::unique_ptr<ProjectGE::GeometryPack> m_TriPack;
	std::unique_ptr<ProjectGE::GeometryPack> m_SquarePack;

	std::unique_ptr<ProjectGE::ShaderArguement> m_Arg1;

	ProjectGE::OrthoCamera m_Cam;
};
class TestZone : public ProjectGE::Application {
public:
	TestZone() {
		PushLayer(new ExampleLayer());
	}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new TestZone();
}
