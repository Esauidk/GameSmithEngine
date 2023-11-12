#include "gtest/gtest.h"
#include "GameSmithEngineTest/TestInclude.h"

int main(int argc, char** argv) {
	// Library depends on this being initialized
	GameSmith::Log::Init();

	// Initializaing Google Test
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}