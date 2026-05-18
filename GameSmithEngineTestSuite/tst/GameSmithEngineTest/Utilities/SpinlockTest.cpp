#include "gtest/gtest.h"
#include "GameSmithEngine/Utilities/Locks/Spinlock.h"

TEST(SpinlockTest, InitiallyUnlocked) {
	GameSmith::Spinlock lock;
	EXPECT_FALSE(lock.IsLocked());
}

TEST(SpinlockTest, LockAndUnlock) {
	GameSmith::Spinlock lock;
	lock.Lock();
	EXPECT_TRUE(lock.IsLocked());
	lock.Unlock();
	EXPECT_FALSE(lock.IsLocked());
}

TEST(SpinlockTest, UnlockWithoutLock) {
	GameSmith::Spinlock lock;
	EXPECT_NO_THROW(lock.Unlock());
	EXPECT_FALSE(lock.IsLocked());
}
