#include "TestRenderLayer.h"

#include "imgui.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/SLab/SLab.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"

TestRenderLayer::TestRenderLayer() : Layer("TestRender"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f), m_PerpCam(glm::pi<float>() / 3, (float)GameSmith::Application::Get().GetWindow().GetWidth(), (float)GameSmith::Application::Get().GetWindow().GetHeight()) {
	char buffer[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::string(buffer).find_last_of("\\");
	auto vertex = std::string(buffer).substr(0, pos).append("\\SampleVertexShader.cso");
	auto pixel = std::string(buffer).substr(0, pos).append("\\BlinnPhongPixelShader.cso");
	auto hull = std::string(buffer).substr(0, pos).append("\\BasicSubdivideHS.cso");
	auto domain = std::string(buffer).substr(0, pos).append("\\BasicRenderDS.cso");

	GameSmith::RendererAPI* renderAPI = GameSmith::RenderingManager::GetInstance()->GetRenderAPI();

	m_VShader = renderAPI->LoadShader(vertex);
	m_PShader = renderAPI->LoadShader(pixel);
	auto hullShader = renderAPI->LoadShader(hull);
	auto domainShader = renderAPI->LoadShader(domain);


	GameSmith::VertexStruct triVertex[] = {
		{ {-0.5f, -0.5f, 0.0f}}, // 0
		{ {0.0f,  0.5f, 0.0f}}, // 1 
		{ {0.5f,  -0.5f, 0.0f}} // 2
	};

	GameSmith::VertexStruct squareVertex[] = {
		{{-0.75f, -0.75f, -0.5f}, {0, 1}},
		{{0.75f, -0.75f, -0.5f}, {1, 1}},
		{{0.75f,  0.75f, -0.5f},{1, 0}},
		{{-0.75f,  0.75f, -0.5f}, {0, 0}},
		{{-0.75f, -0.75f, 0.5f}, {0, 1}},
		{{0.75f, -0.75f, 0.5f}, {1, 1}},
		{{0.75f,  0.75f, 0.5f},{1, 0}},
		{{-0.75f,  0.75f, 0.5f}, {0, 0}}
	};

	vBuff = renderAPI->CreateVertexBuffer((BYTE*)&squareVertex, sizeof(GameSmith::VertexStruct), _countof(squareVertex));

	unsigned int indexCount[] = {
		0, 1, 2
	};


	unsigned int squareIndex[] = {
		2,1,0,
		0,3,2,
		4,7,0,
		0,7,3,
		1,6,5,
		1,2,6
	};

	iBuff = renderAPI->CreateIndexBuffer((unsigned int*)&squareIndex, _countof(squareIndex));

	renderAPI->SetVertexBuffer(vBuff);
	renderAPI->SetIndexBuffer(iBuff);

	/*GameSmith::DirectX12Shader(GameSmith::CompileShaderForDX12("struct VertexInput{float3 Position : POSITION;};struct VertexShaderOutput{float4 Position : SV_POSITION;};VertexShaderOutput main(VertexInput input){VertexShaderOutput vso;vso.Position = float4(input.Position, 1);return vso;}",
		"main", GameSmith::STAGE_VERTEX, "test"));*/

	//GameSmith::SLabMetadata metadata;
	auto pModel = GameSmith::Ref<GameSmith::ShaderParameter>(new GameSmith::ShaderParameterMatrix("Model"));
	auto pColor = GameSmith::Ref<GameSmith::ShaderParameter>(new GameSmith::ShaderParameterFloat3("InputColor"));

	//renderAPI->SetConstantBuffer(cBuff1, GameSmith::STAGE_DOMAIN, GameSmith::ShaderConstantType::Global);

	m_Sampler = renderAPI->CreateSampler(GameSmith::FilterType::Point, GameSmith::PaddingMethod::Clamp);
	renderAPI->SetSampler(m_Sampler, GameSmith::STAGE_PIXEL);

	auto texture = std::string(buffer).substr(0, pos).append("\\download.png");
	auto instance = GameSmith::ResourceManager::GetInstance();
	m_Tex2d = instance->GetResource<GameSmith::TextureAsset>(texture);

	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ShaderParameter>> params;
	params.insert({ pModel->GetName(), pModel });
	params.insert({ pColor->GetName(), pColor });

	std::unordered_map<std::string, GameSmith::Ref<GameSmith::TextureAsset>> texs;
	texs.insert({ texture, m_Tex2d });

	std::vector<std::string> parameterOrder;
	parameterOrder.push_back(pModel->GetName());
	parameterOrder.push_back(pColor->GetName());

	std::vector<std::string> textureOrder;
	textureOrder.push_back(texture);

	GameSmith::ShaderSet sSet;
	GameSmith::MaterialConfig config;
	//sSet.shaders[GameSmith::STAGE_HULL] = hullShader;
	//sSet.shaders[GameSmith::STAGE_DOMAIN] = domainShader;
	sSet.shaders[GameSmith::STAGE_VERTEX] = GameSmith::Ref<GameSmith::ShaderAsset>(new GameSmith::ShaderAsset(m_VShader));
	sSet.shaders[GameSmith::STAGE_PIXEL] = GameSmith::Ref<GameSmith::ShaderAsset>(new GameSmith::ShaderAsset(m_PShader));

	m_Mat = GameSmith::Ref<GameSmith::Material>(new GameSmith::Material(sSet, config, parameterOrder, textureOrder, params, texs));
	m_Mat->ApplyMaterial();

	m_CopyMat = GameSmith::Ref<GameSmith::Material>(new GameSmith::Material(*m_Mat));
}

void TestRenderLayer::OnImGuiRender() {
	ImGui::Begin("Camera Setting");
	ImGui::Checkbox("Perspective Camera", &switchPerp);
	ImGui::Separator();
	ImGui::Text("Light Settings");
	ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));
	ImGui::Checkbox("Point Light", &switchLight);
	ImGui::InputFloat3("Direction Light: Direction", glm::value_ptr(lightDir));
	ImGui::InputFloat3("Point Light: Position", glm::value_ptr(lightPos));
	ImGui::End();
}

void TestRenderLayer::OnUpdate() {
	float dt = GameSmith::Application::Get().GetTimer().GetDeltaTimeSeconds();
	GameSmith::Transform& tf = m_Cam.GetTransform();
	GameSmith::Transform& perTf = m_PerpCam.GetTransform();

	glm::vec3 oldPos = tf.GetPosition();
	glm::vec3 TriOldPos = m_TriTrans.GetPosition();
	if (GameSmith::Input::IsKeyPressed(GE_KEY_A)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(-1, 0, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_D)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(1, 0, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_W)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(0, 1, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_S)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(0, -1, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_R)) {
		glm::vec3 oldRotation = tf.GetRotation();
		tf.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_J)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(-1, 0, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_L)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(1, 0, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_I)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 1, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_K)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, -1, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_O)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 0, 1);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_P)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 0, -1);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_M)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_N)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 1, 0));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_B)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, -1, 0));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_Z)) {
		glm::vec3 oldScale = m_TriTrans.GetScale();
		m_TriTrans.SetScale(oldScale + dt * glm::vec3(-1, -1, -1));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_X)) {
		glm::vec3 oldScale = m_TriTrans.GetScale();
		m_TriTrans.SetScale(oldScale + dt * glm::vec3(1, 1, 1));
	}

	glm::mat4 tri = m_TriTrans.GetModelMatrix();
	//glm::mat4 squ = glm::transpose(m_SquareTrans.GetModelMatrix());

	auto model = m_Mat->GetParameter<GameSmith::ShaderParameterMatrix>("Model");
	auto color = m_Mat->GetParameter<GameSmith::ShaderParameterFloat3>("InputColor");

	model->SetData(tri);
	glm::vec3 newColor(1, 0, 1);
	color->SetData(newColor);
	m_Mat->ApplyMaterial();

	//std::dynamic_pointer_cast<GameSmith::DirectX12Texture2D>(m_Tex2d)->Test();

	auto renderManager = GameSmith::RenderingManager::GetInstance();
	GameSmith::DirectionalLight light;
	light.SetLightColor(lightColor);
	light.SetLightDirection(lightDir);

	GameSmith::PointLight pLight;
	pLight.SetLightPosition(lightPos);
	pLight.SetLightColor(lightColor);

	if (!switchPerp) {
		if (!switchLight) {
			renderManager->BeginScene(&m_Cam, &light);
		}
		else {
			renderManager->BeginScene(&m_Cam, &pLight);
		}
	}
	else {
		if (!switchLight) {
			renderManager->BeginScene(&m_PerpCam, &light);
		}
		else {
			renderManager->BeginScene(&m_PerpCam, &pLight);
		}
		
	}
	//renderAPI->SetVertexBuffer(vBuff);
	//renderAPI->SetIndexBuffer(iBuff);

	//renderAPI->DrawIndexed(iBuff->GetCount(), 1);
	
}