#include "gtest/gtest.h"
#include "GameSmithEngine/Core/MixStack.h"

static constexpr int NORMAL_FIRST = 1;
static constexpr int NORMAL_SECOND = 2;
static constexpr int OVERLAY_FIRST = 99;
static constexpr int OVERLAY_SECOND = 100;
static constexpr int GHOST_VALUE = 42;

TEST(MixStackTest, PushAndIterate) {
	GameSmith::MixStack<int> stack;
	stack.Push(new int(NORMAL_FIRST));
	stack.Push(new int(NORMAL_SECOND));
	auto it = stack.begin();
	EXPECT_EQ(**it, NORMAL_FIRST);
	++it;
	EXPECT_EQ(**it, NORMAL_SECOND);
}

TEST(MixStackTest, PushSpecialGoesToEnd) {
	GameSmith::MixStack<int> stack;
	stack.Push(new int(NORMAL_FIRST));
	stack.PushSpecial(new int(OVERLAY_FIRST));
	auto it = stack.begin();
	EXPECT_EQ(**it, NORMAL_FIRST);
	++it;
	EXPECT_EQ(**it, OVERLAY_FIRST);
}

TEST(MixStackTest, PushSpecialAlwaysAfterNormal) {
	GameSmith::MixStack<int> stack;
	stack.PushSpecial(new int(OVERLAY_FIRST));
	stack.Push(new int(NORMAL_FIRST));
	stack.Push(new int(NORMAL_SECOND));
	stack.PushSpecial(new int(OVERLAY_SECOND));
	auto it = stack.begin();
	EXPECT_EQ(**it, NORMAL_FIRST); ++it;
	EXPECT_EQ(**it, NORMAL_SECOND); ++it;
	EXPECT_EQ(**it, OVERLAY_FIRST); ++it;
	EXPECT_EQ(**it, OVERLAY_SECOND);
}

TEST(MixStackTest, PopRemovesNormalItem) {
	GameSmith::MixStack<int> stack;
	int* a = new int(NORMAL_FIRST);
	int* b = new int(NORMAL_SECOND);
	stack.Push(a);
	stack.Push(b);
	stack.Pop(a);
	delete a;
	auto it = stack.begin();
	EXPECT_EQ(**it, NORMAL_SECOND);
}

TEST(MixStackTest, PopSpecialRemovesOverlayItem) {
	GameSmith::MixStack<int> stack;
	int* a = new int(NORMAL_FIRST);
	int* ov = new int(OVERLAY_FIRST);
	stack.Push(a);
	stack.PushSpecial(ov);
	stack.PopSpecial(ov);
	delete ov;
	auto it = stack.begin();
	EXPECT_EQ(**it, NORMAL_FIRST);
	++it;
	EXPECT_EQ(it, stack.end());
}

TEST(MixStackTest, EmptyStackBeginEqualsEnd) {
	GameSmith::MixStack<int> stack;
	EXPECT_EQ(stack.begin(), stack.end());
}

TEST(MixStackTest, PopNonExistentItemDoesNothing) {
	GameSmith::MixStack<int> stack;
	int* a = new int(NORMAL_FIRST);
	stack.Push(a);
	int* ghost = new int(GHOST_VALUE);
	EXPECT_NO_THROW(stack.Pop(ghost));
	EXPECT_NE(stack.begin(), stack.end());
	delete ghost;
}

TEST(MixStackTest, PopSpecialNonExistentDoesNothing) {
	GameSmith::MixStack<int> stack;
	int* a = new int(NORMAL_FIRST);
	stack.Push(a);
	int* ghost = new int(GHOST_VALUE);
	EXPECT_NO_THROW(stack.PopSpecial(ghost));
	EXPECT_NE(stack.begin(), stack.end());
	delete ghost;
}
