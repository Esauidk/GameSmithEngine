#include "gtest/gtest.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateInitializer.h"

TEST(PipelineStateInitializerTest, DefaultValues) {
	GameSmith::PipelineStateInitializer init;
	EXPECT_FALSE(init.tesselation);
	EXPECT_EQ(init.numRT, (unsigned int)1);
	EXPECT_EQ(init.toplopgyType, GameSmith::TopologyType::Triangle);
}

TEST(PipelineStateInitializerTest, Equality) {
	GameSmith::PipelineStateInitializer a;
	GameSmith::PipelineStateInitializer b;
	EXPECT_EQ(a, b);
}

TEST(PipelineStateInitializerTest, InequalityTesselation) {
	GameSmith::PipelineStateInitializer a;
	GameSmith::PipelineStateInitializer b;
	b.tesselation = true;
	EXPECT_NE(a, b);
}

TEST(PipelineStateInitializerTest, InequalityTopology) {
	GameSmith::PipelineStateInitializer a;
	GameSmith::PipelineStateInitializer b;
	b.toplopgyType = GameSmith::TopologyType::Point;
	EXPECT_NE(a, b);
}

TEST(PipelineStateInitializerTest, InequalityNumRT) {
	GameSmith::PipelineStateInitializer a;
	GameSmith::PipelineStateInitializer b;
	b.numRT = 4;
	EXPECT_NE(a, b);
}

TEST(PipelineStateInitializerTest, HashConsistent) {
	GameSmith::PipelineStateInitializer init;
	GameSmith::PipelineStateInitializer hasher;
	std::size_t h1 = hasher(init);
	std::size_t h2 = hasher(init);
	EXPECT_EQ(h1, h2);
}

TEST(PipelineStateInitializerTest, HashDiffersForDifferentTesselation) {
	GameSmith::PipelineStateInitializer a;
	GameSmith::PipelineStateInitializer b;
	b.tesselation = true;
	GameSmith::PipelineStateInitializer hasher;
	EXPECT_NE(hasher(a), hasher(b));
}

TEST(PipelineStateInitializerTest, HashDiffersForDifferentTopology) {
	GameSmith::PipelineStateInitializer a;
	GameSmith::PipelineStateInitializer b;
	b.toplopgyType = GameSmith::TopologyType::Square;
	GameSmith::PipelineStateInitializer hasher;
	EXPECT_NE(hasher(a), hasher(b));
}
