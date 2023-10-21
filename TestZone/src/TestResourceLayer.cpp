#include "TestResourceLayer.h"
#include "imgui.h"

#include "ProjectGE/ResourceManagement/ResourceAssetHelper.h"

TestResourceLayer::TestResourceLayer()
{
	ProjectGE::ResourceAssetWriter writer(30);

	std::string st1("Hello there");
	struct Test {
		unsigned int v1 = 5;
		int v2 = 2;
		float v3 = 3.5f;
	}ex1;
	
	writer.WriteString(st1);
	writer.WriteData<Test>(&ex1);

	ProjectGE::ResourceAssetReader reader(writer.GetBuffer(), writer.GetBufferSize());
	std::string st2 = reader.GetString();
	Test* ex2 = reader.ReadData<Test>();

	auto instance = ProjectGE::ResourceManager::GetInstance();
	m_Resource = instance->GetResource<ProjectGE::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\hat_LP.obj");
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
