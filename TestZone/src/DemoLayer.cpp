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

	GameSmith::ContainerDataType dataType = GameSmith::ContainerDataType::Matrix;
	writer.WriteClass<GameSmith::ContainerDataType>(&dataType);

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
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType);
	writer1.WriteClass<glm::mat4>(&mat);

	GameSmith::ContainerDataType dataType1 = GameSmith::ContainerDataType::Float4;

	paramName = "Color1";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	glm::vec4 color1(0.0f, 1.0f, 0.0f, 1.0f);
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color2";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	color1 = { 1.0f, 0.0f, 0.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	 paramName = "Color3";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	color1 = { 0.0f, 0.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color4";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	color1 = { 1.0f, 1.0f, 0.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color5";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	color1 = { 1.0f, 0.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color6";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	color1 = { 0.0f, 1.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color7";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	color1 = { 1.0f, 1.0f, 1.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	paramName = "Color8";
	writer1.WriteString(paramName);
	writer1.WriteClass<GameSmith::ContainerDataType>(&dataType1);
	color1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	writer1.WriteClass<glm::vec4>(&color1);

	m_ColorMatAsset = instance->GetResource<GameSmith::MaterialAsset>("ColorMat", writer1.GetBuffer(), writer1.GetBufferSize());

	GameSmith::ResourceAssetWriter writer2(700);
	meta.TetureCount = 1;
	meta.ParamterCount = 1;
	writer2.WriteClass<GameSmith::MaterialAssetMetadata>(&meta);
	writer2.WriteClass<GameSmith::MaterialConfig>(&config);
	writer2.WriteString(vs);
	
	std::string ps2("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\SamplePixelShader.cso");
	writer2.WriteString(ps2);

	paramName = "Model";
	writer2.WriteString(paramName);
	writer2.WriteClass<GameSmith::ContainerDataType>(&dataType);
	writer2.WriteClass<glm::mat4>(&mat);

	paramName = "texture";
	writer2.WriteString(paramName);
	std::string tex("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\thief.png");
	writer2.WriteString(tex);

	m_LummieThiefAsset = instance->GetResource<GameSmith::MaterialAsset>("LummieAsset", writer2.GetBuffer(), writer2.GetBufferSize());
	auto render = GameSmith::RenderingManager::GetInstance()->GetRenderAPI();
	auto sampler = render->CreateSampler(GameSmith::FilterType::Linear, GameSmith::PaddingMethod::Clamp);
	render->SetSampler(sampler, GameSmith::Stages::STAGE_PIXEL);

	GameSmith::ResourceAssetWriter writer3(700);
	meta.TetureCount = 0;
	meta.ParamterCount = 1;
	writer3.WriteClass<GameSmith::MaterialAssetMetadata>(&meta);
	writer3.WriteClass<GameSmith::MaterialConfig>(&config);
	writer3.WriteString(vs);

	std::string ps3("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\FlatColorPixelShader.cso");
	writer3.WriteString(ps3);

	paramName = "Model";
	writer3.WriteString(paramName);
	writer3.WriteClass<GameSmith::ContainerDataType>(&dataType);
	writer3.WriteClass<glm::mat4>(&mat);

	auto LightAsset = instance->GetResource<GameSmith::MaterialAsset>("LightAsset", writer3.GetBuffer(), writer3.GetBufferSize());

	m_LightObject = GameSmith::GameObjectManager::GetInstance()->CreateGameObject(m_LightPos, glm::vec3(0, 0, 0));
	auto meshRe = m_LightObject.lock()->AddComponent<GameSmith::MeshRenderer>();
	auto mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\sphere.obj");
	meshRe.lock()->SetMesh(mesh);

	for (unsigned int i = 0; i < meshRe.lock()->GetMaterialSlots(); i++) {
		GameSmith::Ref<GameSmith::Material> matInstance = LightAsset->CreateInstance();

		meshRe.lock()->SetMaterial(i, matInstance);
	}
}

void DemoLayer::OnImGuiRender()
{
	ImGui::Begin("Demo Setting");
	ImGui::Text("Simulation Setting");
	ImGui::SliderFloat("Time Scale", &m_TimeScale, 0, 2);
	ImGui::Separator();
	ImGui::Text("Game Object Setting");
	const char* Meshs[] = { "Custom Mesh", "Cybertruck", "Box", "Hat", "Pipe", "Icoshedron" };
	ImGui::ListBox("Select GameObject", &m_MeshItem, Meshs, 6);
	ImGui::InputText("Mesh Location", m_MeshLocation, 500);
	const char* Shaders[] = { "Blue Spread", "BlinnPhong", "LummieThief"};
	ImGui::ListBox("Select Shader", &m_ShaderItem, Shaders, 3);
	m_CreateGameObject = ImGui::Button("Spawn GameObject");
	m_ClearGameObjects = ImGui::Button("Clear GameObject");
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

	if (m_ClearGameObjects) {
		auto manager = GameSmith::GameObjectManager::GetInstance();
		
		for (auto object : m_GameObjects) {
			manager->DestroyGameObject(object);
		}

		m_GameObjects.clear();
	}

	m_LightObject.lock()->GetTransform().lock()->SetPosition(m_LightPos);

	if (m_CreateGameObject) {
		auto object = GameSmith::GameObjectManager::GetInstance()->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(adist(rng), adist(rng), adist(rng)));
		auto meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
		auto demoScript = object.lock()->AddComponent<DemoScript>();
		demoScript.lock()->SetOribitSpeeds(glm::vec3(ddist(rng), ddist(rng), ddist(rng)));
		demoScript.lock()->SetLocalRotationSpeed(glm::vec3(odist(rng), odist(rng), odist(rng)));
		demoScript.lock()->SetOrigin(glm::vec3(0, 0, 20));
		demoScript.lock()->SetSphereSize(rdist(rng));

		GameSmith::Ref<GameSmith::MeshAsset> mesh;
		switch (m_MeshItem) {
		case 0:
			mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>(m_MeshLocation);
			break;
		case 1:
			mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\cybertruck.obj");
			break;
		case 2:
			mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\test.obj");
			break;
		case 3:
			mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\hat_LP.obj");
			break;
		case 4:
			mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\pvc_pipe_90.obj");
			break;
		case 5:
			mesh = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\icosahedron.obj");
			break;
		}

		meshRef.lock()->SetMesh(mesh);

		for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
			GameSmith::Ref<GameSmith::Material> matInstance;
			
			switch (m_ShaderItem) {
			case 0:
				matInstance = m_ColorMatAsset->CreateInstance();
				break;
			case 1:
				matInstance = m_LightMatAsset->CreateInstance();
				break;
			case 2:
				matInstance = m_LummieThiefAsset->CreateInstance();
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
