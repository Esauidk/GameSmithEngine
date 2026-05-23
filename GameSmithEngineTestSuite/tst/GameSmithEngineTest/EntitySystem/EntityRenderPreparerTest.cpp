#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"

class EntityRenderPreparerTest : public ::testing::Test {
protected:
	void SetUp() override {
		GameSmith::EntityRenderPreparer::Init();
	}

	void TearDown() override {
		GameSmith::EntityRenderPreparer::Shutdown();
	}
};

TEST_F(EntityRenderPreparerTest, InitAndShutdown) {
	auto* instance = GameSmith::EntityRenderPreparer::GetInstance();
	EXPECT_NE(instance, nullptr);
}

TEST_F(EntityRenderPreparerTest, AddRenderRequestNoCrash) {
	auto* prep = GameSmith::EntityRenderPreparer::GetInstance();
	GameSmith::RenderRequest req{nullptr, nullptr, nullptr, GameSmith::Transform()};
	EXPECT_NO_THROW(prep->AddRenderRequest(req));
}

TEST_F(EntityRenderPreparerTest, SetMainCameraNoCrash) {
	auto* prep = GameSmith::EntityRenderPreparer::GetInstance();
	GameSmith::RenderableCamera cam{nullptr, nullptr};
	EXPECT_NO_THROW(prep->SetMainCamera(cam));
}
