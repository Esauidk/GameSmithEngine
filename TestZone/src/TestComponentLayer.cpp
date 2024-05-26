#include "TestComponentLayer.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

TestComponentLayer::TestComponentLayer() : GameSmith::Layer("Component System Layer"), gameObject("Test Object") {
	
}

void TestComponentLayer::OnAttach()
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

	auto matInstance = asset->CreateInstance();

	auto trans = gameObject.GetTransform();
	auto meshRend = gameObject.AddComponent<GameSmith::MeshRenderer>();
	auto testComp = gameObject.AddComponent<GameSmith::TestComponent>();

	meshRend.lock()->SetMesh(mesh);
	
	for (unsigned int i = 0; i < meshRend.lock()->GetMaterialSlots(); i++) {
		meshRend.lock()->SetMaterial(i, matInstance);
	}

	glm::vec3 pos = trans.lock()->GetPosition();
	GE_APP_INFO("GameObject location: x:{0}, y{1}, z{2}", pos.x, pos.y, pos.z);

	gameObject.RemoveComponent(testComp);
}

void TestComponentLayer::OnImGuiRender()
{

}

void TestComponentLayer::OnUpdate()
{

	auto renderManager = GameSmith::RenderingManager::GetInstance();
	renderManager->EndScene();

}
