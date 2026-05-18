#include "gtest/gtest.h"
#include "GameSmithEngine/Utilities/ThreadSafeQueue.h"

TEST(ThreadSafeQueueTest, InitiallyEmpty) {
	GameSmith::ThreadSafeQueue<int> queue;
	EXPECT_TRUE(queue.IsEmpty());
}

TEST(ThreadSafeQueueTest, PushAndPop) {
	GameSmith::ThreadSafeQueue<int> queue;
	queue.Push(42);
	EXPECT_FALSE(queue.IsEmpty());
	int val = queue.Pop();
	EXPECT_EQ(val, 42);
	EXPECT_TRUE(queue.IsEmpty());
}

TEST(ThreadSafeQueueTest, MultiplePushesAndPops) {
	GameSmith::ThreadSafeQueue<int> queue;
	queue.Push(1);
	queue.Push(2);
	queue.Push(3);
	EXPECT_EQ(queue.Pop(), 1);
	EXPECT_EQ(queue.Pop(), 2);
	EXPECT_EQ(queue.Pop(), 3);
	EXPECT_TRUE(queue.IsEmpty());
}

TEST(ThreadSafeQueueTest, PushAndPopStrings) {
	GameSmith::ThreadSafeQueue<std::string> queue;
	queue.Push("hello");
	queue.Push("world");
	EXPECT_EQ(queue.Pop(), "hello");
	EXPECT_EQ(queue.Pop(), "world");
	EXPECT_TRUE(queue.IsEmpty());
}
