#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/Components/MeshRenderer.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "GameSmithEngine/EntitySystem/Transform.h"

TEST(MeshRendererTest, ConstructorDoesNotCrash) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	EXPECT_NO_THROW(GameSmith::MeshRenderer renderer(&gameObj, &transform));
}

TEST(MeshRendererTest, GetName) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::MeshRenderer renderer(&gameObj, &transform);
	EXPECT_EQ(renderer.GetName(), "MeshRenderer");
}

TEST(MeshRendererTest, GetPriority) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::MeshRenderer renderer(&gameObj, &transform);
	EXPECT_EQ(renderer.GetPriority(), 0);
}

TEST(MeshRendererTest, OnUpdateWithNullDependencies) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::MeshRenderer renderer(&gameObj, &transform);
	EXPECT_NO_THROW(renderer.OnUpdate(0.016f));
}

TEST(MeshRendererTest, PostRegistryBootstrapDoesNotCrash) {
	GameSmith::Transform transform;
	GameSmith::GameObject gameObj("Test");
	GameSmith::MeshRenderer renderer(&gameObj, &transform);
	EXPECT_NO_THROW(renderer.PostRegistryBootstrap());
}
