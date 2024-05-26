#include "SandBoxLayer.h"

#include "imgui.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/DirectX12RenderTexture.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/DirectX12Texture2D.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"

#include "GameSmithEngine/Core/Log.h"

SandBoxLayer::SandBoxLayer() : Layer("SandBox"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f), m_PerpCam((float)GameSmith::Application::Get().GetWindow()->GetWidth(), (float)GameSmith::Application::Get().GetWindow()->GetHeight(), glm::pi<float>() / 3, 0, 500.0f),
renderWidth((float)GameSmith::Application::Get().GetWindow()->GetWidth()),
renderHeight((float)GameSmith::Application::Get().GetWindow()->GetHeight()),
renderX(0),
renderY(0)
{
	GameSmith::ResourceAssetWriter writer(500);
	/*GameSmith::MaterialAssetMetadata meta;
	meta.ParamterCount = 1;
	meta.TetureCount = 0;
	meta.Shaders[GameSmith::Stages::STAGE_VERTEX].UsedShader = true;
	meta.Shaders[GameSmith::Stages::STAGE_PIXEL].UsedShader = true;
	writer.WriteClass<GameSmith::MaterialAssetMetadata>(&meta);*/

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

	GameSmith::Ref<GameSmith::MaterialAsset> asset = instance->GetResource<GameSmith::MaterialAsset>("MaterialComp", writer.GetBuffer(), writer.GetBufferSize());
	auto mesh = instance->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\cybertruck.obj");

	std::vector<GameSmith::Connection<GameSmith::GameObject>> gameObjectPartition1;
	std::vector<GameSmith::Connection<GameSmith::GameObject>> gameObjectPartition2;

	auto gameObjectManager = GameSmith::GameObjectManager::GetInstance();

	auto object = gameObjectManager->CreateGameObject(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0));
	auto meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
	meshRef.lock()->SetMesh(mesh);
	
	for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
		auto matInstance = asset->CreateInstance();
		meshRef.lock()->SetMaterial(i, matInstance);
	}

	m_t1 = object.lock()->GetTransform();
	gameObjectPartition1.push_back(object);

	object = gameObjectManager->CreateGameObject(glm::vec3(-1, 0, 10), glm::vec3(0, 0, 0));
	meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
	meshRef.lock()->SetMesh(mesh);
	for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
		auto matInstance = asset->CreateInstance();
		meshRef.lock()->SetMaterial(i, matInstance);
	}

	m_t2 = object.lock()->GetTransform();
	gameObjectPartition1.push_back(object);

	object = gameObjectManager->CreateGameObject(glm::vec3(9, 1, 20), glm::vec3(0, 0, 0));
	meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
	meshRef.lock()->SetMesh(mesh);
	for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
		auto matInstance = asset->CreateInstance();
		meshRef.lock()->SetMaterial(i, matInstance);
	}

	m_t3 = object.lock()->GetTransform();
	gameObjectPartition2.push_back(object);

	auto renderManager = GameSmith::RenderingManager::GetInstance();
	m_RenderTex = renderManager->GetRenderAPI()->CreateRenderTexture((float)GameSmith::Application::Get().GetWindow()->GetWidth(), (float)GameSmith::Application::Get().GetWindow()->GetHeight());
	m_GameTex = renderManager->GetRenderAPI()->CreateRenderTexture((float)GameSmith::Application::Get().GetWindow()->GetWidth(), (float)GameSmith::Application::Get().GetWindow()->GetHeight());
	
	GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_RenderTex.get()), false);
	GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_GameTex.get()), false);
	renderManager->GetRenderAPI()->SetRenderTexture(m_RenderTex, 0);
	renderManager->SetFrameTexture(m_RenderTex);
	auto sceneManager = GameSmith::ChunkManager::GetInstance();

	

	m_OutsideAsset = instance->GetResource<GameSmith::TextureAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\download.png");
	m_OutsideTex = m_OutsideAsset->GetTexture();

	m_GameView = GameSmith::Application::Get().GetImGuiInstance()->GenerateTextureSpace(m_GameTex);
	GameSmith::Ref<GameSmith::GameChunk> chunk1 = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(gameObjectPartition1));
	m_Chunk2 = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(gameObjectPartition2));
	sceneManager->LoadChunk("Sample Chunk1", chunk1);
}

void SandBoxLayer::OnImGuiRender()
{
	ImGui::Begin("Camera Setting");
	ImGui::Checkbox("Perspective Camera", &switchPerp);
	ImGui::Separator();
	ImGui::Text("Light Settings");
	ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));
	ImGui::Checkbox("Point Light", &switchLight);
	ImGui::InputFloat3("Direction Light: Direction", glm::value_ptr(lightDir));
	ImGui::InputFloat3("Point Light: Position", glm::value_ptr(lightPos));
	ImGui::Text("GameObject Settings");
	ImGui::InputFloat3("GameObject 1 Position", glm::value_ptr(pos1));
	ImGui::InputFloat3("GameObject 1 Rotation", glm::value_ptr(rot1));
	ImGui::InputFloat3("GameObject 2 Position", glm::value_ptr(pos2));
	ImGui::InputFloat3("GameObject 2 Rotation", glm::value_ptr(rot2));
	ImGui::InputFloat3("GameObject 3 Position", glm::value_ptr(pos3));
	ImGui::InputFloat3("GameObject 3 Rotation", glm::value_ptr(rot3));
	applyPartition2 =  ImGui::Button("Insert Partition 2");
	ImGui::Text("Partial Screen Render Setting");
	ImGui::Checkbox("Render Partial Screen", &doPartialRendering);
	ImGui::InputFloat("RenderX", &renderX);
	ImGui::InputFloat("RenderY", &renderY);
	ImGui::InputFloat("RenderWidth", &renderWidth);
	ImGui::InputFloat("RenderHeight", &renderHeight);
	ImGui::Text("Hello");
	/*auto d3Tex = GameSmith::CastPtr<GameSmith::DirectX12RenderTexture>(m_Tex);
	d3Tex->ChangeState(GameSmith::RTState::READ);*/
	ImGui::Image((ImTextureID)(m_GameView.gpuSpot), ImVec2(m_GameTex->GetWidth() / 2, m_GameTex->GetHeight()/2));
	ImGui::Text("Hello");
	ImGui::End();
}

void SandBoxLayer::OnUpdate()
{
	auto renderManager = GameSmith::RenderingManager::GetInstance();
	renderManager->SetForClear(m_RenderTex);

	m_RenderTex->CopyTexture(m_GameTex);

	GameSmith::DirectionalLight light;
	light.SetLightColor(lightColor);
	light.SetLightDirection(lightDir);

	GameSmith::PointLight pLight;
	pLight.SetLightPosition(lightPos);
	pLight.SetLightColor(lightColor);

	if (doPartialRendering) {
		auto window = GameSmith::Application::Get().GetWindow();

		if (window->IsRenderingFullWindow()) {
			window->SetFullWindowRender(false);
		}

		m_PerpCam.SetWidth(renderWidth);
		m_PerpCam.SetHeight(renderHeight);
		window->ChangeRenderLocation(renderX, renderY, renderWidth, renderHeight);
	}
	else {
		auto window = GameSmith::Application::Get().GetWindow();
		if (!window->IsRenderingFullWindow()) {
			window->SetFullWindowRender(true);
			m_PerpCam.SetWidth(window->GetWidth());
			m_PerpCam.SetHeight(window->GetHeight());
		}
	}

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

	if (applyPartition2 && m_Chunk2 != nullptr) {
		auto sceneManager = GameSmith::ChunkManager::GetInstance();
		sceneManager->LoadChunk("Sample Chunk2", m_Chunk2);
		m_Chunk2 = nullptr;
	}

	if (!m_t1.expired()) {
		m_t1.lock()->SetPosition(pos1);
		m_t1.lock()->SetRotation(rot1);
	}

	if (!m_t2.expired()) {
		m_t2.lock()->SetPosition(pos2);
		m_t2.lock()->SetRotation(rot2);
	}
	
	if (!m_t3.expired()) {
		m_t3.lock()->SetPosition(pos3);
		m_t3.lock()->SetRotation(rot3);
	}
	
	//renderManager->EndScene();
}
