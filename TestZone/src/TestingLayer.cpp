#include "TestingLayer.h"

TestingLayer::TestingLayer() : GameSmith::Layer("Testing Layer")
{
	std::vector<std::pair<std::string, GameSmith::Stages>> shaderPaths;
	std::vector<std::pair<std::string, std::string>> texturePaths;
	std::vector<std::pair<std::string, GameSmith::ContainerDataType>> variables;

	shaderPaths.push_back({ "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso", GameSmith::Stages::STAGE_VERTEX });
	shaderPaths.push_back({ "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\RandomColorPS.cso", GameSmith::Stages::STAGE_PIXEL });

	variables.push_back({ "Model", GameSmith::ContainerDataType::Matrix });
	variables.push_back({ "Color1", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color2", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color3", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color4", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color5", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color6", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color7", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color8", GameSmith::ContainerDataType::Float4 });

	GameSmith::MaterialAsset testMat(shaderPaths, texturePaths, variables);
	testMat.Serialize();
	auto mat1 = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MaterialAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\TestMat.mat");
	int i = 0;
	
}

void TestingLayer::OnImGuiRender()
{
}

void TestingLayer::OnUpdate()
{
}
