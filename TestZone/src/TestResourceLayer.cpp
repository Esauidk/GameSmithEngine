#include "TestResourceLayer.h"
#include "imgui.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

TestResourceLayer::TestResourceLayer()
{
	GameSmith::ResourceAssetWriter writer(500);
	/*GameSmith::MaterialAssetMetadata meta;
	meta.ParamterCount = 1;
	meta.TetureCount = 1;
	meta.Shaders[GameSmith::Stages::STAGE_VERTEX].UsedShader = true;
	writer.WriteClass<GameSmith::MaterialAssetMetadata>(&meta);*/

	GameSmith::MaterialConfig config;
	writer.WriteClass<GameSmith::MaterialConfig>(&config);

	std::string vs("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso");
	writer.WriteString(vs);

	std::string paramName("color");
	writer.WriteString(paramName);

	GameSmith::ContainerDataType dataType = GameSmith::ContainerDataType::Float3;
	writer.WriteClass<GameSmith::ContainerDataType>(&dataType);

	glm::vec3 mat(2, 5, 1);
	writer.WriteClass<glm::vec3>(&mat);

	std::string texParamName("texture");
	writer.WriteString(texParamName);

	std::string tex("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\download.png");
	writer.WriteString(tex);


	auto instance = GameSmith::ResourceManager::GetInstance();

	GameSmith::Ref<GameSmith::MaterialAsset> asset = instance->GetResource<GameSmith::MaterialAsset>("Material", writer.GetBuffer(), writer.GetBufferSize());

	m_Resource = instance->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\cybertruck.obj");
	m_TexResource = instance->GetResource<GameSmith::TextureAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\download.png");

}

void TestResourceLayer::OnImGuiRender()
{
	ImGui::Begin("Resource Manager Settings");
	m_Pressed = ImGui::Button("Destroy Resource");
	ImGui::End();
}

void TestResourceLayer::OnUpdate()
{

	if (m_Pressed) {
		m_TexResource = nullptr;
	}
	else if(m_TexResource == nullptr) {
		auto instance = GameSmith::ResourceManager::GetInstance();
		m_TexResource = instance->GetResource<GameSmith::TextureAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\download.png");
	}

	/*m_Resource->SetGraphicsMesh();
	for (unsigned int i = 0; i < m_Resource->GetSubMeshSize(); i++) {
		m_Resource->DrawMesh(i);
	}*/
	//GameSmith::RenderingManager::GetInstance()->GetRenderAPI()->SubmitRecording();
	
}
