#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/GameplayUpdater.h"
#include "Components/TestComponent.h"

TEST(GameplayUpdaterTest, ComponentUpdate) {
	GameSmith::GameplayUpdater::Init();
	GameSmith::GameplayUpdater* updater = GameSmith::GameplayUpdater::GetInstance();

	auto testComp = GameSmith::Ref<TestComponent>(new TestComponent(nullptr, nullptr));

	EXPECT_FALSE(testComp->GetUpdateCheck());

	updater->Register(testComp);
	updater->FlushRegistration();
	updater->RunGameplayUpdate(0);

	EXPECT_TRUE(testComp->GetUpdateCheck());
	GameSmith::GameplayUpdater::ShutDown();
}

TEST(GameplayUpdaterTest, ComponentRegistrationMidUpdate) {
	GameSmith::GameplayUpdater::Init();
	GameSmith::GameplayUpdater* updater = GameSmith::GameplayUpdater::GetInstance();

	auto testComp = GameSmith::Ref<TestComponent>(new TestComponent(nullptr, nullptr));

	class FakeComponent : public GameSmith::Component {
	public:
		FakeComponent(
			GameSmith::GameObject* gameObject, 
			GameSmith::Transform* transform, 
			GameSmith::GameplayUpdater* update, 
			GameSmith::Ref<GameSmith::Component> comp) : Component(gameObject, transform), m_Update(update), m_Comp(comp), m_UpdateRan(false) { }

		virtual void OnStart() override { }
		virtual void OnUpdate(float dt) { m_Update->Register(m_Comp); m_UpdateRan = true; }
		virtual void OnDestroy() override {}
		bool GetUpdateCheck() { return m_UpdateRan; }
		virtual std::string GetName() const override { return "Fake Component"; };
		virtual int GetPriority() const { return 0; }
	private:
		GameSmith::GameplayUpdater* m_Update;
		GameSmith::Ref<GameSmith::Component> m_Comp;
		bool m_UpdateRan;
	};

	EXPECT_FALSE(testComp->GetUpdateCheck());

	auto testComp2 = GameSmith::Ref<FakeComponent>(new FakeComponent(nullptr, nullptr, updater, testComp));
	EXPECT_FALSE(testComp2->GetUpdateCheck());

	updater->Register(testComp2);
	updater->FlushRegistration();
	ASSERT_NO_THROW(updater->RunGameplayUpdate(0));
	EXPECT_TRUE(testComp2->GetUpdateCheck());

	updater->FlushRegistration();
	updater->RunGameplayUpdate(0);
	EXPECT_TRUE(testComp->GetUpdateCheck());

	GameSmith::GameplayUpdater::ShutDown();
}

TEST(GameplayUpdaterTest, ComponentInit) {
	GameSmith::GameplayUpdater::Init();
	GameSmith::GameplayUpdater* updater = GameSmith::GameplayUpdater::GetInstance();

	auto testComp = GameSmith::Ref<TestComponent>(new TestComponent(nullptr, nullptr));

	EXPECT_FALSE(testComp->GetUpdateCheck());

	updater->Register(testComp);
	updater->FlushRegistration();
	updater->RunGameplayInit();

	EXPECT_TRUE(testComp->GetInitCheck());
	GameSmith::GameplayUpdater::ShutDown();
}

TEST(GameplayUpdaterTest, NoExceptionOnComponentRemoval) {
	GameSmith::GameplayUpdater::Init();
	GameSmith::GameplayUpdater* updater = GameSmith::GameplayUpdater::GetInstance();

	auto testComp = GameSmith::Ref<TestComponent>(new TestComponent(nullptr, nullptr));

	EXPECT_FALSE(testComp->GetUpdateCheck());

	updater->Register(testComp);
	testComp = nullptr;

	EXPECT_NO_THROW(updater->RunGameplayUpdate(0));
	GameSmith::GameplayUpdater::ShutDown();
}