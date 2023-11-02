#include "TestComponentLayer.h"
#include "ProjectGE/Core/Log.h"

#include "ProjectGE/ResourceManagement/ResourceAssetHelper.h"

TestComponentLayer::TestComponentLayer() : ProjectGE::Layer("Component System Layer") {
	
}

void TestComponentLayer::OnAttach()
{

	ProjectGE::ResourceAssetWriter writer(500);
	ProjectGE::MaterialAssetMetadata meta;
	meta.ParamterCount = 1;
	meta.TetureCount = 0;
	meta.Shaders[ProjectGE::Stages::STAGE_VERTEX].UsedShader = true;
	meta.Shaders[ProjectGE::Stages::STAGE_PIXEL].UsedShader = true;
	writer.WriteClass<ProjectGE::MaterialAssetMetadata>(&meta);

	ProjectGE::MaterialConfig config;
	writer.WriteClass<ProjectGE::MaterialConfig>(&config);

	std::string vs("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso");
	writer.WriteString(vs);

	std::string ps("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\BlinnPhongPixelShader.cso");
	writer.WriteString(ps);

	std::string paramName("Model");
	writer.WriteString(paramName);

	ProjectGE::ShaderDataType dataType = ProjectGE::ShaderDataType::Matrix;
	writer.WriteClass<ProjectGE::ShaderDataType>(&dataType);

	glm::mat4 mat;
	writer.WriteClass<glm::mat4>(&mat);


	auto instance = ProjectGE::ResourceManager::GetInstance();

	ProjectGE::Ref<ProjectGE::MaterialAsset> asset = instance->GetResource<ProjectGE::MaterialAsset>("MaterialComp", writer.GetBuffer(), writer.GetBufferSize());
	auto mesh = instance->GetResource<ProjectGE::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\cybertruck.obj");

	auto matInstance = asset->CreateInstance();

	auto trans = gameObject.GetTransform();
	auto meshRend = gameObject.AddComponent<ProjectGE::MeshRenderer>();

	meshRend->SetMesh(mesh);
	
	for (unsigned int i = 0; i < meshRend->GetMaterialSlots(); i++) {
		meshRend->SetMaterial(i, matInstance);
	}

	glm::vec3 pos = trans->GetPosition();
	GE_APP_INFO("GameObject location: x:{0}, y{1}, z{2}", pos.x, pos.y, pos.z);
}

void TestComponentLayer::OnImGuiRender()
{

}

void TestComponentLayer::OnUpdate()
{
	gameObject.OnUpdate();

	auto renderManager = ProjectGE::RenderingManager::GetInstance();
	renderManager->EndScene();

}
