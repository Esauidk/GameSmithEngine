#include "SandBoxLayer.h"

#include "imgui.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

#include "GameSmithEngine/Core/Log.h"

SandBoxLayer::SandBoxLayer() : Layer("SandBox"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f), m_PerpCam(glm::pi<float>() / 3, (float)GameSmith::Application::Get().GetWindow().GetWidth(), (float)GameSmith::Application::Get().GetWindow().GetHeight())
{
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

	GameSmith::Ref<GameSmith::MaterialAsset> asset = instance->GetResource<GameSmith::MaterialAsset>("MaterialComp", writer.GetBuffer(), writer.GetBufferSize());
	auto mesh = instance->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\cybertruck.obj");

	auto gameObjectManager = GameSmith::GameObjectManager::GetInstance();

	auto object = gameObjectManager->CreateGameObject(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0));
	auto meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
	meshRef.lock()->SetMesh(mesh);
	
	for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
		auto matInstance = asset->CreateInstance();
		meshRef.lock()->SetMaterial(i, matInstance);
	}

	m_GameObjects.push_back(object);

	object = gameObjectManager->CreateGameObject(glm::vec3(-1, 0, 10), glm::vec3(0, 0, 0));
	meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
	meshRef.lock()->SetMesh(mesh);
	for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
		auto matInstance = asset->CreateInstance();
		meshRef.lock()->SetMaterial(i, matInstance);
	}

	m_GameObjects.push_back(object);

	object = gameObjectManager->CreateGameObject(glm::vec3(9, 1, 20), glm::vec3(0, 0, 0));
	meshRef = object.lock()->AddComponent<GameSmith::MeshRenderer>();
	meshRef.lock()->SetMesh(mesh);
	for (unsigned int i = 0; i < meshRef.lock()->GetMaterialSlots(); i++) {
		auto matInstance = asset->CreateInstance();
		meshRef.lock()->SetMaterial(i, matInstance);
	}

	m_GameObjects.push_back(object);
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
	ImGui::End();
}

void SandBoxLayer::OnUpdate()
{
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

	m_GameObjects[0].lock()->GetTransform()->SetPosition(pos1);
	m_GameObjects[0].lock()->GetTransform()->SetRotation(rot1);
	m_GameObjects[1].lock()->GetTransform()->SetPosition(pos2);
	m_GameObjects[1].lock()->GetTransform()->SetRotation(rot2);
	m_GameObjects[2].lock()->GetTransform()->SetPosition(pos3);
	m_GameObjects[2].lock()->GetTransform()->SetRotation(rot3);

	/*m_GameObjects[2].lock()->OnUpdate();
	m_GameObjects[1].lock()->OnUpdate();
	m_GameObjects[0].lock()->OnUpdate();*/
	
	//renderManager->EndScene();
}
