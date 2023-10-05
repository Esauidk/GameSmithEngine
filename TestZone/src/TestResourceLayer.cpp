#include "TestResourceLayer.h"
#include "imgui.h"

TestResourceLayer::TestResourceLayer()
{
	auto instance = ProjectGE::ResourceManager::GetInstance();
	//m_Resource = instance->GetResource<ProjectGE::TestResource>("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\download.png");
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
	
}
