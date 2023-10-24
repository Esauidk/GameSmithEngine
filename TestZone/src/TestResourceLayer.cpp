#include "TestResourceLayer.h"
#include "imgui.h"

#include "ProjectGE/ResourceManagement/ResourceAssetHelper.h"

TestResourceLayer::TestResourceLayer()
{
	ProjectGE::ResourceAssetWriter writer(500);
	ProjectGE::MaterialAssetMetadata meta;
	meta.ParamterCount = 1;
	meta.TetureCount = 1;
	meta.Shaders[ProjectGE::Stages::STAGE_VERTEX].UsedShader = true;
	writer.WriteClass<ProjectGE::MaterialAssetMetadata>(&meta);

	ProjectGE::MaterialConfig config;
	writer.WriteClass<ProjectGE::MaterialConfig>(&config);

	std::string vs("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso");
	writer.WriteString(vs);

	std::string paramName("color");
	writer.WriteString(paramName);

	ProjectGE::ShaderDataType dataType = ProjectGE::ShaderDataType::Float3;
	writer.WriteClass<ProjectGE::ShaderDataType>(&dataType);

	glm::vec3 mat(2, 5, 1);
	writer.WriteClass<glm::vec3>(&mat);

	std::string texParamName("texture");
	writer.WriteString(texParamName);

	std::string tex("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\download.png");
	writer.WriteString(tex);


	auto instance = ProjectGE::ResourceManager::GetInstance();

	ProjectGE::Ref<ProjectGE::MaterialAsset> asset = instance->GetResource<ProjectGE::MaterialAsset>("Material", writer.GetBuffer(), writer.GetBufferSize());

	m_Resource = instance->GetResource<ProjectGE::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\cybertruck.obj");
	m_TexResource = instance->GetResource<ProjectGE::TextureAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\download.png");

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
		auto instance = ProjectGE::ResourceManager::GetInstance();
		m_TexResource = instance->GetResource<ProjectGE::TextureAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\download.png");
	}

	m_Resource->SetGraphicsMesh();
	ProjectGE::RenderingManager::GetInstance()->GetRenderAPI()->SubmitRecording();
	
}
