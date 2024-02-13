#include "DemoLayer.h"

#include "imgui.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

DemoLayer::DemoLayer() : GameSmith::Layer("Demo Layer"), m_PerpCam(glm::pi<float>() / 3, (float)GameSmith::Application::Get().GetWindow()->GetWidth(), (float)GameSmith::Application::Get().GetWindow()->GetHeight())
{
	auto renderManager = GameSmith::RenderingManager::GetInstance();
	float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
	m_RenderTex = renderManager->GetRenderAPI()->CreateRenderTexture((float)GameSmith::Application::Get().GetWindow()->GetWidth(), (float)GameSmith::Application::Get().GetWindow()->GetHeight(), color);
	GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_RenderTex.get()), false);
	renderManager->GetRenderAPI()->SetRenderTexture(m_RenderTex, 0);
	renderManager->SetFrameTexture(m_RenderTex);

	GameSmith::ResourceAssetWriter writer(500);
	GameSmith::MaterialAssetMetadata meta;
	meta.ParamterCount = 1;
	meta.TetureCount = 0;
	meta.Shaders[GameSmith::Stages::STAGE_VERTEX].UsedShader = true;
	meta.Shaders[GameSmith::Stages::STAGE_PIXEL].UsedShader = true;
	writer.WriteClass<GameSmith::MaterialAssetMetadata>(&meta);

	GameSmith::MaterialConfig config;
	writer.WriteClass<GameSmith::MaterialConfig>(&config);

	std::string vs("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso");
	writer.WriteString(vs);

	std::string ps("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\BlinnPhongPixelShader.cso");
	writer.WriteString(ps);

	std::string paramName("Model");
	writer.WriteString(paramName);

	GameSmith::ShaderDataType dataType = GameSmith::ShaderDataType::Matrix;
	writer.WriteClass<GameSmith::ShaderDataType>(&dataType);

	glm::mat4 mat;
	writer.WriteClass<glm::mat4>(&mat);


	auto instance = GameSmith::ResourceManager::GetInstance();

	m_LightMatAsset = instance->GetResource<GameSmith::MaterialAsset>("MaterialComp", writer.GetBuffer(), writer.GetBufferSize());

	GameSmith::ResourceAssetWriter writer1(700);
	meta.ParamterCount = 9;
	writer1.WriteClass<GameSmith::MaterialAssetMetadata>(&meta);
	writer1.WriteClass<GameSmith::MaterialConfig>(&config);
	writer1.WriteString(vs);

	std::string ps1("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\RandomColorPS.cso");
	writer1.WriteString(ps1);

	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType);
	writer1.WriteClass<glm::mat4>(&mat);

	GameSmith::ShaderDataType dataType1 = GameSmith::ShaderDataType::Float4;

	paramName = "Color1";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	glm::vec4 color1(0.0f, 1.0f, 0.0f, 1.0f);
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color2";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	color1 = { 1.0f, 0.0f, 0.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	 paramName = "Color3";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	color1 = { 0.0f, 0.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color4";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	color1 = { 1.0f, 1.0f, 0.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color5";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	color1 = { 1.0f, 0.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color6";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	color1 = { 0.0f, 1.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color7";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	color1 = { 1.0f, 1.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color8";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ShaderDataType>(&dataType1);
	color1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	m_ColorMatAsset = instance->GetResource<GameSmith::MaterialAsset>("ColorMat", writer1.GetBuffer(), writer1.GetBufferSize());
}

void DemoLayer::OnImGuiRender()
{
	ImGui::Begin("Demo Setting");
	ImGui::Text("Simulation Setting");
	ImGui::SliderFloat("Time Scale", &m_TimeScale, 0, 2);
	ImGui::Separator();
	ImGui::Text("Game Object Setting");
	ImGui::InputText("Mesh Location", m_MeshLocation, 500);
	const char* Shaders[] = { "Blue Spread", "BlinnPhong" };
	ImGui::ListBox("Select Shader", &m_ShaderItem, Shaders, 2);
	m_CreateGameObject = ImGui::Button("Spawn GameObject");
	ImGui::Separator();
	ImGui::Text("Light Settings");
	ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
	ImGui::Checkbox("Point Light", &m_SwitchLight);
	ImGui::InputFloat3("Direction Light: Direction", glm::value_ptr(m_LightDir));
	ImGui::InputFloat3("Point Light: Position", glm::value_ptr(m_LightPos));
	ImGui::End();
}

void DemoLayer::OnUpdate()
{
	auto renderManager = GameSmith::RenderingManager::GetInstance();
	renderManager->SetForClear(m_RenderTex);

	if (m_CreateGameObject) {
		auto object = GameSmith::GameObjectManager::GetInstance()->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(adist(rng), adist(rng), adist(rng)));
		auto meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
		auto demoScript = object.lock()->AddComponent<DemoScript>();
		demoScript.lock()->SetOribitSpeeds(glm::vec3(ddist(rng), ddist(rng), ddist(rng)));
		demoScript.lock()->SetLocalRotationSpeed(glm::vec3(odist(rng), odist(rng), odist(rng)));
		demoScript.lock()->SetOrigin(glm::vec3(0, 0, 20));
		demoScript.lock()->SetSphereSize(rdist(rng));

		auto mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>(m_MeshLocation);
		meshRef.lock()->SetMesh(mesh);

		for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
			GameSmith::Ref<GameSmith::Material> matInstance;
			
			switch (m_ShaderItem) {
			case 0:
				matInstance = m_ColorMatAsset->CreateInstance();
				break;
			case 1:
				matInstance = m_LightMatAsset->CreateInstance();
			}

			meshRef.lock()->SetMaterial(i, matInstance);
		}

		m_GameObjects.push_back(object);
	}

	for (auto gameObject : m_GameObjects) {
		gameObject.lock()->GetComponent<DemoScript>().lock()->SetTimeScale(m_TimeScale);
	}

	GameSmith::DirectionalLight light;
	light.SetLightColor(m_LightColor);
	light.SetLightDirection(m_LightDir);

	GameSmith::PointLight pLight;
	pLight.SetLightPosition(m_LightPos);
	pLight.SetLightColor(m_LightColor);

	if (!m_SwitchLight) {
		renderManager->BeginScene(&m_PerpCam, &light);
	}
	else {
		renderManager->BeginScene(&m_PerpCam, &pLight);
	}

	renderManager->EndScene();
}

void DemoScript::OnUpdate(float dt)
{
	auto orbit = m_t.GetRotation();
	orbit += m_OribitSpeeds * dt * m_TimeScale;
	orbit = glm::mod(orbit, 2 * glm::pi<float>());
	m_t.SetRotation(orbit);
	auto localRot = m_Transform->GetRotation();
	localRot += m_LocalSpeeds * dt * m_TimeScale;
	localRot = glm::mod(localRot, 2 * glm::pi<float>());
	m_Transform->SetRotation(localRot);
}
