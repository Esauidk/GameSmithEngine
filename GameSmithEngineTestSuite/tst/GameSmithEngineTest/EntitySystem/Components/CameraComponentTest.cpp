#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/Components/CameraComponent.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "GameSmithEngine/EntitySystem/Transform.h"
#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/PerspectiveCamera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/OrthoCamera.h"

TEST(CameraComponentTest, GetCameraReturnsNonNull) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);
	EXPECT_NE(camComp.GetCamera(), nullptr);
}

TEST(CameraComponentTest, DefaultCameraTypeIsPerspective) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);
	auto* cam = camComp.GetCamera();
	EXPECT_NE(dynamic_cast<GameSmith::PerspectiveCamera*>(cam), nullptr);
}

TEST(CameraComponentTest, ChangeToOrthographic) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);
	camComp.ChangeCameraType(GameSmith::CameraType::Orthographic);
	auto* cam = camComp.GetCamera();
	EXPECT_NE(dynamic_cast<GameSmith::OrthoCamera*>(cam), nullptr);
}

TEST(CameraComponentTest, ChangeBackToPerspective) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);
	camComp.ChangeCameraType(GameSmith::CameraType::Orthographic);
	camComp.ChangeCameraType(GameSmith::CameraType::Perspective);
	auto* cam = camComp.GetCamera();
	EXPECT_NE(dynamic_cast<GameSmith::PerspectiveCamera*>(cam), nullptr);
}

TEST(CameraComponentTest, GetName) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);
	EXPECT_EQ(camComp.GetName(), "CameraComponent");
}

TEST(CameraComponentTest, GetPriority) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);
	EXPECT_EQ(camComp.GetPriority(), 0);
}

TEST(CameraComponentTest, OnUpdateWithEntityRenderPreparer) {
	GameSmith::EntityRenderPreparer::Init();
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);

	EXPECT_NO_THROW(camComp.OnUpdate(0.016f));

	GameSmith::EntityRenderPreparer::Shutdown();
}

TEST(CameraComponentTest, FirstCameraLinkedToParentTransform) {
	GameSmith::Transform parentTransform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &parentTransform);

	auto& camTransform = camComp.GetCamera()->GetTransform();
	glm::mat4 before = camTransform.GetModelMatrix();

	parentTransform.SetPosition(glm::vec3(10, 20, 30));

	glm::mat4 after = camTransform.GetModelMatrix();
	EXPECT_NE(before, after);
}

TEST(CameraComponentTest, SecondCameraParentTransformIndependent) {
	GameSmith::Transform parent1;
	GameSmith::Transform parent2;
	GameSmith::GameObject obj1("Test1");
	GameSmith::GameObject obj2("Test2");

	GameSmith::CameraComponent cam1(&obj1, &parent1);

	GameSmith::CameraComponent cam2(&obj2, &parent2);
	auto& cam2Transform = cam2.GetCamera()->GetTransform();
	glm::mat4 before = cam2Transform.GetModelMatrix();

	parent1.SetPosition(glm::vec3(100, 200, 300));

	glm::mat4 after = cam2Transform.GetModelMatrix();
	EXPECT_EQ(before, after);
}

TEST(CameraComponentTest, FirstCameraIsActive) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);

	EXPECT_NE(GameSmith::CameraComponent::GetActiveCamera(), nullptr);
}

TEST(CameraComponentTest, ActiveCameraChangesAfterSetAsActive) {
	GameSmith::Transform transform1;
	GameSmith::Transform transform2;
	GameSmith::GameObject obj1("Test1");
	GameSmith::GameObject obj2("Test2");

	GameSmith::CameraComponent cam1(&obj1, &transform1);
	cam1.SetAsActiveCamera();

	GameSmith::CameraComponent cam2(&obj2, &transform2);

	EXPECT_EQ(GameSmith::CameraComponent::GetActiveCamera(), &cam1);

	cam2.SetAsActiveCamera();
	EXPECT_EQ(GameSmith::CameraComponent::GetActiveCamera(), &cam2);
}

TEST(CameraComponentTest, SetAsMainCameraDoesNotCrash) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::CameraComponent camComp(&gameObj, &transform);

	EXPECT_NO_THROW(camComp.SetAsActiveCamera());
}

TEST(CameraComponentTest, MultipleCamerasDoNotCrash) {
	GameSmith::Transform transform1;
	GameSmith::Transform transform2;
	GameSmith::GameObject gameObj1("Test1");
	GameSmith::GameObject gameObj2("Test2");

	GameSmith::CameraComponent camComp1(&gameObj1, &transform1);
	GameSmith::CameraComponent camComp2(&gameObj2, &transform2);

	EXPECT_NE(camComp1.GetCamera(), nullptr);
	EXPECT_NE(camComp2.GetCamera(), nullptr);
}
