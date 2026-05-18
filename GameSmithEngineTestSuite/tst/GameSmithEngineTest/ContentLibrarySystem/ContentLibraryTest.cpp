#include "gtest/gtest.h"
#include "GameSmithEngine/ContentLibrarySystem/DefaultContentLibrary.h"
#include "GameSmithEngine/ContentLibrarySystem/ContentLibrary.h"

TEST(ContentLibraryTest, DefaultContentLibraryInitAndShutdown) {
	GameSmith::DefaultContentLibrary lib;
	EXPECT_NO_THROW(lib.Init());
	EXPECT_NO_THROW(lib.Shutdown());
}

TEST(ContentLibraryTest, ContentLibraryIsDerivedFromBase) {
	GameSmith::DefaultContentLibrary lib;
	GameSmith::ContentLibrary* base = &lib;
	EXPECT_NE(base, nullptr);
}
