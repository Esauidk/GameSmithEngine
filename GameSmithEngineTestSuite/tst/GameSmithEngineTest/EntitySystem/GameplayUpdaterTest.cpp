#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/GameplayUpdater.h"
#include "Components/TestComponent.h"

TEST(GameplayUpdaterTest, ComponentUpdate) {
	GameSmith::GameplayUpdater updater;

	auto testComp = GameSmith::Ref<TestComponent>(new TestComponent(nullptr, nullptr));

	EXPECT_FALSE(testComp->GetUpdateCheck());

	updater.Register(testComp);
	updater.RunGameplayUpdate();

	EXPECT_TRUE(testComp->GetUpdateCheck());
}

TEST(GameplayUpdaterTest, ComponentInit) {
	GameSmith::GameplayUpdater updater;

	auto testComp = GameSmith::Ref<TestComponent>(new TestComponent(nullptr, nullptr));

	EXPECT_FALSE(testComp->GetUpdateCheck());

	updater.Register(testComp);
	updater.RunGameplayInit();

	EXPECT_TRUE(testComp->GetInitCheck());
}

TEST(GameplayUpdaterTest, NoExceptionOnComponentRemoval) {
	GameSmith::GameplayUpdater updater;

	auto testComp = GameSmith::Ref<TestComponent>(new TestComponent(nullptr, nullptr));

	EXPECT_FALSE(testComp->GetUpdateCheck());

	updater.Register(testComp);
	testComp = nullptr;

	EXPECT_NO_THROW(updater.RunGameplayUpdate());
}