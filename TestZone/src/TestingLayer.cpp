#include "TestingLayer.h"

TestingLayer::TestingLayer() : GameSmith::Layer("Testing Layer")
{


	auto id1 = GameSmith::ResourceManager::GetInstance()->ImportResource("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\TestMat.mat");
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

	auto mat1 = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MaterialAsset>(testMat->GetId());
	int i = 0;

	
	i = 1;
}

void TestingLayer::OnImGuiRender()
{
}

void TestingLayer::OnUpdate()
{
}
