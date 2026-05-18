#include "gtest/gtest.h"
#include "GameSmithEngine/Utilities/ThreadSafeVector.h"

TEST(ThreadSafeVectorTest, InitiallyEmpty) {
	GameSmith::ThreadSafeVector<int> vec;
	EXPECT_TRUE(vec.isEmpty());
	EXPECT_EQ(vec.Size(), (size_t)0);
}

TEST(ThreadSafeVectorTest, PushBackAndSize) {
	GameSmith::ThreadSafeVector<int> vec;
	vec.PushBack(10);
	EXPECT_FALSE(vec.isEmpty());
	EXPECT_EQ(vec.Size(), (size_t)1);
}

TEST(ThreadSafeVectorTest, GetReturnsPushedValue) {
	GameSmith::ThreadSafeVector<int> vec;
	vec.PushBack(42);
	EXPECT_EQ(vec.Get(0), 42);
}

TEST(ThreadSafeVectorTest, MultipleElements) {
	GameSmith::ThreadSafeVector<int> vec;
	vec.PushBack(1);
	vec.PushBack(2);
	vec.PushBack(3);
	EXPECT_EQ(vec.Size(), (size_t)3);
	EXPECT_EQ(vec.Get(0), 1);
	EXPECT_EQ(vec.Get(1), 2);
	EXPECT_EQ(vec.Get(2), 3);
}

TEST(ThreadSafeVectorTest, RemoveShrinks) {
	GameSmith::ThreadSafeVector<int> vec;
	vec.PushBack(10);
	vec.PushBack(20);
	vec.PushBack(30);
	vec.Remove(1);
	EXPECT_EQ(vec.Size(), (size_t)2);
	EXPECT_EQ(vec.Get(0), 10);
	EXPECT_EQ(vec.Get(1), 30);
}
