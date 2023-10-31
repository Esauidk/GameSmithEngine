#include "TestComponentLayer.h"
#include "ProjectGE/Core/Log.h"

TestComponentLayer::TestComponentLayer() : ProjectGE::Layer("Component System Layer") {
	
}

void TestComponentLayer::OnAttach()
{
	auto trans = gameObject.GetComponent<ProjectGE::Transform>();
	glm::vec3 pos = trans->GetPosition();
	GE_APP_INFO("GameObject location: x:{0}, y{1}, z{2}", pos.x, pos.y, pos.z);
}

void TestComponentLayer::OnImGuiRender()
{
}

void TestComponentLayer::OnUpdate()
{
}
