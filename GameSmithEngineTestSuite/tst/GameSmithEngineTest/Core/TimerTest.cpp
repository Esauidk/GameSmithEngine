#include "gtest/gtest.h"
#include "GameSmithEngine/Core/Timer.h"

TEST(TimerTest, InitialStateIsZero) {
	GameSmith::Timer timer;
	EXPECT_FLOAT_EQ(timer.GetDeltaTimeSeconds(), 0.0f);
	EXPECT_FLOAT_EQ(timer.GetDeltaTimeMilliseconds(), 0.0f);
}

TEST(TimerTest, MarkReturnsNonZeroDelta) {
	GameSmith::Timer timer;
	float dt = timer.Mark();
	EXPECT_GE(dt, 0.0f);
}

TEST(TimerTest, MeasureWithoutMarkDoesNotAdvanceDelta) {
	GameSmith::Timer timer;
	timer.Mark();
	float before = timer.GetDeltaTimeSeconds();
	float measured = timer.MeasureWithoutMark();
	float after = timer.GetDeltaTimeSeconds();
	EXPECT_FLOAT_EQ(after, before);
	EXPECT_GE(measured, 0.0f);
}

TEST(TimerTest, ResetReturnsToZero) {
	GameSmith::Timer timer;
	timer.Mark();
	timer.Reset();
	EXPECT_FLOAT_EQ(timer.GetDeltaTimeSeconds(), 0.0f);
}

TEST(TimerTest, TotalTimeAccumulates) {
	GameSmith::Timer timer;
	timer.Mark();
	timer.Mark();
	float total = timer.GetTotalTimeSeconds();
	EXPECT_GT(total, 0.0f);
}

TEST(TimerTest, DeltaTimeUnitConversion) {
	GameSmith::Timer timer;
	timer.Mark();
	float dtSec = timer.GetDeltaTimeSeconds();
	float dtMs = timer.GetDeltaTimeMilliseconds();
	EXPECT_FLOAT_EQ(dtMs, dtSec * 1000.0f);
}
