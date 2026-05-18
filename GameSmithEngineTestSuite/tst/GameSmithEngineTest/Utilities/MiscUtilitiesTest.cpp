#include "gtest/gtest.h"
#include "GameSmithEngine/Utilities/MiscellaneousUtilities.h"

TEST(MiscUtilitiesTest, ToWideString) {
	std::string narrow = "Hello";
	std::wstring wide = GameSmith::ToWideString(narrow);
	EXPECT_EQ(wide, L"Hello");
}

TEST(MiscUtilitiesTest, ToNarrowString) {
	std::wstring wide = L"World";
	std::string narrow = GameSmith::ToNarrowString(wide);
	EXPECT_EQ(narrow.size(), wide.size());
	EXPECT_EQ(narrow, "World");
}

TEST(MiscUtilitiesTest, RoundTrip) {
	std::string original = "GameSmithEngine Test!";
	std::wstring wide = GameSmith::ToWideString(original);
	std::string roundTrip = GameSmith::ToNarrowString(wide);
	EXPECT_EQ(original, roundTrip);
}

TEST(MiscUtilitiesTest, EmptyString) {
	std::string empty;
	std::wstring wide = GameSmith::ToWideString(empty);
	EXPECT_TRUE(wide.empty());
	std::string back = GameSmith::ToNarrowString(wide);
	EXPECT_TRUE(back.empty());
}
