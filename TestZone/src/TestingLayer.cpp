#include "TestingLayer.h"

TestingLayer::TestingLayer() : GameSmith::Layer("Testing Layer")
{


	auto id2 = GameSmith::ResourceManager::GetInstance()->ImportResource("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso");
	auto id3 = GameSmith::ResourceManager::GetInstance()->ImportResource("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\RandomColorPS.cso");

	std::vector<std::pair<GameSmith::ID, GameSmith::Stages>> shaderIds;
	std::vector<std::pair<std::string, GameSmith::ID>> textureIds;
	std::vector<std::pair<std::string, GameSmith::ContainerDataType>> variables;

	shaderIds.push_back({ id2, GameSmith::Stages::STAGE_VERTEX });
	shaderIds.push_back({ id3, GameSmith::Stages::STAGE_PIXEL });

	variables.push_back({ "Model", GameSmith::ContainerDataType::Matrix });
	variables.push_back({ "Color1", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color2", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color3", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color4", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color5", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color6", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color7", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color8", GameSmith::ContainerDataType::Float4 });

	GameSmith::Ref<GameSmith::MaterialAsset> testMat = GameSmith::Ref<GameSmith::MaterialAsset>(new GameSmith::MaterialAsset(shaderIds, textureIds, variables));
	testMat->Serialize();

	GameSmith::ResourceManager::GetInstance()->WriteResource(testMat, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\TestFileMat.mat");

	int i = 0;

	auto gm = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	auto meshRend = gm.lock()->AddComponent<GameSmith::MeshRenderer>().lock();


	auto meshID = GameSmith::ResourceManager::GetInstance()->ImportResource("C:\\Users\\esaus\\Documents\\Coding Projects\\test.obj");
	auto meshAsset = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>(meshID);
	auto matAsset = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MaterialAsset>(testMat->GetId());
	meshRend->SetMesh(meshAsset);

	for (unsigned int i = 0; i < meshRend->GetMaterialSlots(); i++) {
		meshRend->SetMaterial(i, matAsset);
	}

	GameSmith::ResourceManager::GetInstance()->WriteResource(meshRend, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\MeshRender.bin");

	auto serial = meshRend->Serialize();


	auto gm1 = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	auto meshRend1 = gm.lock()->AddComponent<GameSmith::MeshRenderer>().lock();
	meshRend1->Deserialize(serial.get(), meshRend->RequireSpace());
	i = 1;
}

void TestingLayer::OnImGuiRender()
{
}

void TestingLayer::OnUpdate()
{
}
