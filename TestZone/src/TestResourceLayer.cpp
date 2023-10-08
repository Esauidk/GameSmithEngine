#include "TestResourceLayer.h"
#include "imgui.h"

TestResourceLayer::TestResourceLayer()
{
	ProjectGE::VertexStruct triVertex[] = {
		{ {-0.5f, -0.5f, 0.0f}}, // 0
		{ {0.0f,  0.5f, 0.0f}}, // 1 
		{ {0.5f,  -0.5f, 0.0f}} // 2
	};

	unsigned int indexCount[] = {
		0, 1, 2
	};

	ProjectGE::MeshMetadata meta = { _countof(triVertex), 1 };

	UINT bufSize = sizeof(ProjectGE::MeshMetadata) + sizeof(triVertex) + sizeof(indexCount);
	char* Test = new char[bufSize];

	char* cur = Test;
	*((ProjectGE::MeshMetadata*)cur) = meta;
	cur += sizeof(ProjectGE::MeshMetadata);

	for (UINT i = 0; i < _countof(triVertex); i++) {
		*((ProjectGE::VertexStruct*)cur) = triVertex[i];
		cur += sizeof(ProjectGE::VertexStruct);
	}

	for (UINT i = 0; i < _countof(indexCount); i++) {
		*((unsigned short*)cur) = indexCount[i];
		cur += sizeof(unsigned short);
	}

	


	auto instance = ProjectGE::ResourceManager::GetInstance();
	m_Resource = instance->GetResource<ProjectGE::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\ProjectGE\\bin\\Debug-windows-x86_64\\TestZone\\icosahedron.obj");
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
