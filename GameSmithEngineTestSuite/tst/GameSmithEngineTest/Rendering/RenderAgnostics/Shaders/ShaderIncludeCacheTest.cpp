#include "gtest/gtest.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderIncludeCache.h"

TEST(ShaderIncludeCacheTest, AddInclude) {
	GameSmith::ShaderIncludeCache cache;
	GameSmith::ID id(1, 2, 3, 4);
	EXPECT_NO_THROW(cache.AddInclude("Core.hlsl", id));
}
