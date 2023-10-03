#include "TestResourceLayer.h"
#include "imgui.h"

TestResourceLayer::TestResourceLayer(UINT threshold) : m_Threshold(threshold)
{
	auto instance = ProjectGE::ResourceManager::GetInstance();
	m_Resource = instance->GetResource<ProjectGE::TestResource>("C:\\Users\\esaus\\Documents\\Coding Projects\\NaniteRendereringRecreation\\bin\\Debug-windows-x86_64\\TestZone\\download.png");
}

void TestResourceLayer::OnImGuiRender()
{
}

void TestResourceLayer::OnUpdate()
{
	if (m_Iteration == m_Threshold) {
		m_Resource = nullptr;
	}
	m_Iteration++;
}
