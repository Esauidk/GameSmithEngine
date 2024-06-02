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
	auto test = gm.lock()->AddComponent<GameSmith::TestComponent>().lock();

	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> variableEntries;
	test->GenerateVariableEntries(&variableEntries);

	auto entry = variableEntries.find("Test Variable1");
	glm::vec1 val;
	val.x = 10.0f;
	GameSmith::CastPtr<GameSmith::FloatContainer>(entry->second)->SetData(val);

	test->BootstrapRegistry(variableEntries);
	GameSmith::Ref<char> reg = test->SerializeRegistry();

	auto gm1 = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	auto test1 = gm.lock()->AddComponent<GameSmith::TestComponent>().lock();
	test1->DeserializeRegistry(reg.get(), test->RegistrySerializationSize());

	i = 1;
}

void TestingLayer::OnImGuiRender()
{
}

void TestingLayer::OnUpdate()
{
}
