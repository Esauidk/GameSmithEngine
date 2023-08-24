#include <ProjectGE.h>

#include "imgui.h"

#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Shader.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12RootSignatureBuilder.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12RootSignature.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12InputLayout.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12TopologyResource.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public ProjectGE::Layer {
public:
	// DIRECTX12 is ROW MAJOR
	struct test {
		glm::vec3 color;
	};

	ExampleLayer() : Layer("Example"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f) {
		/* START: TEST CODE REMOVE */
		// Read Shaders(Vertex, Pixel)

		m_TriTrans.SetPosition(glm::vec3(0, 1, 0));
		m_SquareTrans.SetPosition(glm::vec3(0, 0, 0));
		ProjectGE::Ref<ProjectGE::DirectX12RootSignature> test = std::make_unique<ProjectGE::DirectX12RootSignature>();
		test->InitGenericRootSignature(D3D12_ROOT_SIGNATURE_FLAG_NONE);
		// Setup Root Signature
		
		//m_Root = ProjectGE::Ref<ProjectGE::ShaderArguementDefiner>(ProjectGE::ShaderArguementDefiner::Create());

		//m_Arg1 = ProjectGE::Ref<ProjectGE::ShaderArguement>(m_Root->AddArguement(ProjectGE::ShaderArguementType::Constant, sizeof(glm::mat4) / 4));
		//m_Arg2 = ProjectGE::Ref<ProjectGE::ShaderArguement>(m_Root->AddArguement(ProjectGE::ShaderArguementType::Constant, sizeof(glm::mat4) / 4));
		//m_Arg3 = ProjectGE::Ref<ProjectGE::ShaderArguement>(m_Root->AddArguement(ProjectGE::ShaderArguementType::ConstantBuffer));


		//m_Root->FinalizeSignature();

		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		auto vertex = std::wstring(buffer).substr(0, pos).append(L"\\/SampleVertexShader.cso");
		auto pixel = std::wstring(buffer).substr(0, pos).append(L"\\/SamplePixelShader.cso");

		std::string nvertex = std::string(vertex.begin(), vertex.end());
		std::string npixel = std::string(pixel.begin(), pixel.end());
		m_Shaders = ProjectGE::Ref<ProjectGE::Shader>(ProjectGE::Shader::Create(nvertex, npixel));

		// Setup Topology

		auto m_Topo = ProjectGE::Topology::Create(ProjectGE::TopologyType::Triangle);

		// Setup Pipeline
		//m_State = ProjectGE::PipelineStateObject::Create();

		//m_Shaders->Append(*(m_State.get()));
		//m_Root->Append(*(m_State.get()));
		//m_Topo->Append(*(m_State.get()));

		ProjectGE::Vertex triVertex[] = {
			{ {-0.5f, -0.5f, 0.0f}}, // 0
			{ {0.0f,  0.5f, 0.0f}}, // 1 
			{ {0.5f,  -0.5f, 0.0f}} // 2
		};

		ProjectGE::Vertex squareVertex[] = {
			{{-0.75f, -0.75f, 0.0f}, {0, 0}},
			{{0.75f, -0.75f, 0.0f}, {1, 0}},
			{{0.75f,  0.75f, 0.0f},{1, 1}},
			{{-0.75f,  0.75f, 0.0f}, {0, 1}}
		};

		ProjectGE::BufferLayoutBuilder layout = { {"POSITION", ProjectGE::ShaderDataType::Float3}, {"UV_TEXCOORD", ProjectGE::ShaderDataType::Float2} };

		auto m_TriVertexBuffer = ProjectGE::VertexBuffer::Create(&triVertex, sizeof(triVertex) / sizeof(ProjectGE::Vertex));
		auto m_SquareVertexBuffer = ProjectGE::VertexBuffer::Create(&squareVertex, sizeof(squareVertex) / sizeof(ProjectGE::Vertex));
		m_TriVertexBuffer->AttachLayout(layout);
		m_SquareVertexBuffer->AttachLayout(layout);

		auto configuredLayout = m_TriVertexBuffer->GetLayout();

		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		ProjectGE::DirectX12PipelineArgs args = {
			{test->GetInternalRootSignature(),
			((ProjectGE::DirectX12InputLayout*)configuredLayout)->GetInternalLayout(),
			((ProjectGE::DirectX12TopologyResource*)m_Topo.get())->GetInternalTolopogyType(),
			CD3DX12_SHADER_BYTECODE(((ProjectGE::DirectX12Shader*)m_Shaders.get())->GetVertexByteCode()),
			CD3DX12_SHADER_BYTECODE(((ProjectGE::DirectX12Shader*)m_Shaders.get())->GetPixelByteCode()),
			DXGI_FORMAT_D32_FLOAT,
			rtvFormats
			} };

		ProjectGE::DirectX12PipelineState state;
		state.Create(args);
		ProjectGE::Ref<ProjectGE::DirectX12PipelineState> stateRef(&state);

		ProjectGE::DirectX12PipelineStateData stateData(stateRef, test);
		ProjectGE::Ref<ProjectGE::DirectX12PipelineStateData> refData(&stateData);

		auto& core = ProjectGE::DirectX12Core::GetCore();
		auto context = core.GetDirectCommandContext();
		context->GetStateManager().SetGraphicsPipelineState(refData);
		context->GetStateManager().BindState();
		context->FinalizeCommandList();
		//configuredLayout->Append(*(m_State.get()));

		//m_State->Build();

		WORD indexCount[] = {
			0, 1, 2
		};

		WORD squareIndex[] = {
			2,1,0,0,3,2
		};

		auto m_TriIndexBuffer = ProjectGE::IndexBuffer::Create(indexCount, (int)_countof(indexCount));
		auto m_SquareIndexBuffer = ProjectGE::IndexBuffer::Create(squareIndex, (int)_countof(squareIndex));

		m_TriPack = ProjectGE::GeometryPack::Create();
		m_TriPack->AttachVertexBuffer(m_TriVertexBuffer);
		m_TriPack->AttachIndexBuffer(m_TriIndexBuffer);
		m_TriPack->AttachTopology(m_Topo);

		m_SquarePack = ProjectGE::GeometryPack::Create();
		m_SquarePack->AttachVertexBuffer(m_SquareVertexBuffer);
		m_SquarePack->AttachIndexBuffer(m_SquareIndexBuffer);
		m_SquarePack->AttachTopology(m_Topo);


		/* END: TEST CODE REMOVE */
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Tint Color", glm::value_ptr(m_Example1.color));
		ImGui::End();
	}


	void OnUpdate() override {
		float dt = ProjectGE::Application::Get().GetTimer().GetDeltaTimeSeconds();
		Transform& tf = m_Cam.GetTransform();

		glm::vec3 oldPos = tf.GetPosition();
		glm::vec3 TriOldPos = m_TriTrans.GetPosition();
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_A)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(-1, 0, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_D)) {
			glm::vec3 newPosition = oldPos + dt * glm::vec3(1, 0, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_W)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(0, 1, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_S)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(0, -1, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_R)) {
			glm::vec3 oldRotation = tf.GetRotation();
			tf.SetRotation(oldRotation + dt * glm::vec3(0,0,1));
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_J)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(-1, 0, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_L)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(1, 0, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_I)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 1, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_K)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, -1, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_P)) {
			glm::vec3 oldRotation = m_TriTrans.GetRotation();
			m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
		}

		ProjectGE::Renderer::BeginScene(m_Cam);
		ProjectGE::Application::Get().GetWindow().GetRenderer()->AttachContextResources();

		//m_State->Bind();
		//m_Root->Bind();
		//std::dynamic_pointer_cast<ProjectGE::DirectX12Shader>(m_Shaders)->UploadShaderInput((BYTE*)&m_Example1);

		glm::mat4 tri = glm::transpose(m_TriTrans.GetModelMatrix());
		glm::mat4 squ = glm::transpose(m_SquareTrans.GetModelMatrix());
		//ProjectGE::Renderer::Submit(m_TriPack, m_Shaders, tri, m_Arg1, m_Arg2);
		//ProjectGE::Renderer::Submit(m_SquarePack, m_Shaders, squ, m_Arg1, m_Arg2);

		ProjectGE::Renderer::EndScene();
	}

	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {}

private:
	//ProjectGE::Ref<ProjectGE::PipelineStateObject> m_State;
	//ProjectGE::Ref<ProjectGE::ShaderArguementDefiner> m_Root;
	ProjectGE::Ref<ProjectGE::Shader> m_Shaders;
	ProjectGE::Ref<ProjectGE::GeometryPack> m_TriPack;
	Transform m_TriTrans;
	ProjectGE::Ref<ProjectGE::GeometryPack> m_SquarePack;
	Transform m_SquareTrans;

	ProjectGE::Ref<ProjectGE::ConstantBuffer> m_C;
	//ProjectGE::Ref<ProjectGE::ShaderArguement> m_Arg1;
	//ProjectGE::Ref<ProjectGE::ShaderArguement> m_Arg2;
	//ProjectGE::Ref<ProjectGE::ShaderArguement> m_Arg3;

	ProjectGE::OrthoCamera m_Cam;
	test m_Example1;
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
