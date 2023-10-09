#include "TestRenderLayer.h"

#include "imgui.h"

#include "ProjectGE/Rendering/RenderAgnostics/Shaders/SLab/SLab.h"
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"

TestRenderLayer::TestRenderLayer() : Layer("TestRender"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f) {
	char buffer[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::string(buffer).find_last_of("\\");
	auto vertex = std::string(buffer).substr(0, pos).append("\\SampleVertexShader.cso");
	auto pixel = std::string(buffer).substr(0, pos).append("\\SamplePixelShader.cso");

	ProjectGE::RendererAPI* renderAPI = ProjectGE::RenderingManager::GetInstance()->GetRenderAPI();

	m_VShader = renderAPI->LoadShader(vertex);
	m_PShader = renderAPI->LoadShader(pixel);


	ProjectGE::VertexStruct triVertex[] = {
		{ {-0.5f, -0.5f, 0.0f}}, // 0
		{ {0.0f,  0.5f, 0.0f}}, // 1 
		{ {0.5f,  -0.5f, 0.0f}} // 2
	};

	ProjectGE::VertexStruct squareVertex[] = {
		{{-0.75f, -0.75f, 0.0f}, {0, 1}},
		{{0.75f, -0.75f, 0.0f}, {1, 1}},
		{{0.75f,  0.75f, 0.0f},{1, 0}},
		{{-0.75f,  0.75f, 0.0f}, {0, 0}}
	};

	vBuff = renderAPI->CreateVertexBuffer((BYTE*)&squareVertex, sizeof(ProjectGE::VertexStruct), _countof(squareVertex));

	ProjectGE::BufferLayoutBuilder layout = { {"POSITION", ProjectGE::ShaderDataType::Float3}, {"UV_TEXCOORD", ProjectGE::ShaderDataType::Float2} };

	vBuff->AttachLayout(layout);

	auto configuredLayout = vBuff->GetLayout();

	ProjectGE::PipelineStateInitializer init;
	init.shaders[ProjectGE::ShaderType::Vertex] = m_VShader;
	init.shaders[ProjectGE::ShaderType::Pixel] = m_PShader;
	init.toplopgyType = ProjectGE::TopologyType::Triangle;
	init.vertexLayout = configuredLayout;

	renderAPI->UpdatePipeline(init);
	ProjectGE::Application::Get().GetWindow().GetRenderer()->AttachContextResources();
	auto top = ProjectGE::TopologyType::Triangle;
	renderAPI->SetTopology(top);

	unsigned int indexCount[] = {
		0, 1, 2
	};


	unsigned int squareIndex[] = {
		2,1,0,0,3,2
	};

	iBuff = renderAPI->CreateIndexBuffer((unsigned int*)&squareIndex, _countof(squareIndex));

	renderAPI->SetVertexBuffer(vBuff);
	renderAPI->SetIndexBuffer(iBuff);

	/*ProjectGE::DirectX12Shader(ProjectGE::CompileShaderForDX12("struct VertexInput{float3 Position : POSITION;};struct VertexShaderOutput{float4 Position : SV_POSITION;};VertexShaderOutput main(VertexInput input){VertexShaderOutput vso;vso.Position = float4(input.Position, 1);return vso;}",
		"main", ProjectGE::STAGE_VERTEX, "test"));*/

	ProjectGE::SLabMetadata metadata;
	metadata.AddParameter(ProjectGE::ShaderParameter("Model", ProjectGE::ShaderDataType::Matrix));
	metadata.AddParameter(ProjectGE::ShaderParameter("InputColor", ProjectGE::ShaderDataType::Float3));

	// TODO: GET RID OF THIS, JUST FOR TESTING WITH SCENE DATA
	cBuff1 = renderAPI->CreateConstantBuffer(sizeof(ProjectGE::GloablShaderData));
	renderAPI->SetConstantBuffer(cBuff1, ProjectGE::STAGE_VERTEX, ProjectGE::ShaderConstantType::Global);

	cBuff2 = renderAPI->CreateConstantBuffer(metadata.GetByteSize());
	renderAPI->SetConstantBuffer(cBuff2, ProjectGE::STAGE_VERTEX, ProjectGE::ShaderConstantType::Instance);
	renderAPI->SetConstantBuffer(cBuff2, ProjectGE::STAGE_PIXEL, ProjectGE::ShaderConstantType::Instance);

	m_Sampler = renderAPI->CreateSampler(ProjectGE::FilterType::Point, ProjectGE::PaddingMethod::Clamp);
	renderAPI->SetSampler(m_Sampler, ProjectGE::STAGE_PIXEL);

	auto texture = std::string(buffer).substr(0, pos).append("\\download.png");
	auto instance = ProjectGE::ResourceManager::GetInstance();
	m_Tex2d = instance->GetResource<ProjectGE::TextureAsset>(texture);
	m_Tex2d->SetGraphicsTexture(); 
}

void TestRenderLayer::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Tint Color", glm::value_ptr(m_Example1.color));
	ImGui::End();
}

void TestRenderLayer::OnUpdate() {
	float dt = ProjectGE::Application::Get().GetTimer().GetDeltaTimeSeconds();
	Transform& tf = m_Cam.GetTransform();

	glm::vec3 oldPos = tf.GetPosition();
	glm::vec3 TriOldPos = m_TriTrans.GetPosition();
	if (ProjectGE::Input::IsKeyPressed(GE_KEY_A)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(-1, 0, 0);
		tf.SetPosition(newPosition);
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_D)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(1, 0, 0);
		tf.SetPosition(newPosition);
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_W)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(0, 1, 0);
		tf.SetPosition(newPosition);
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_S)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(0, -1, 0);
		tf.SetPosition(newPosition);
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_R)) {
		glm::vec3 oldRotation = tf.GetRotation();
		tf.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
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

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_M)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_N)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 1, 0));
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_B)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 1, 0));
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_Z)) {
		glm::vec3 oldScale = m_TriTrans.GetScale();
		m_TriTrans.SetScale(oldScale + dt * glm::vec3(-1, -1, -1));
	}

	if (ProjectGE::Input::IsKeyPressed(GE_KEY_X)) {
		glm::vec3 oldScale = m_TriTrans.GetScale();
		m_TriTrans.SetScale(oldScale + dt * glm::vec3(1, 1, 1));
	}

	glm::mat4 tri = glm::transpose(m_TriTrans.GetModelMatrix());
	//glm::mat4 squ = glm::transpose(m_SquareTrans.GetModelMatrix());
	ProjectGE::GloablShaderData data;
	data.VP = m_Cam.GetMatrix();
	cBuff1->UpdateData((BYTE*)&data, sizeof(data));


	struct instanceData {
		glm::mat4 model;
		glm::vec3 color;
	} test;

	test.model = tri;
	test.color = { 1, 0, 1 };
	cBuff2->UpdateData((BYTE*)&test, sizeof(instanceData));

	//std::dynamic_pointer_cast<ProjectGE::DirectX12Texture2D>(m_Tex2d)->Test();
	ProjectGE::RendererAPI* renderAPI = ProjectGE::RenderingManager::GetInstance()->GetRenderAPI();

	//renderAPI->SetVertexBuffer(vBuff);
	//renderAPI->SetIndexBuffer(iBuff);

	//renderAPI->DrawIndexed(iBuff->GetCount(), 1);
	renderAPI->SubmitRecording();
}