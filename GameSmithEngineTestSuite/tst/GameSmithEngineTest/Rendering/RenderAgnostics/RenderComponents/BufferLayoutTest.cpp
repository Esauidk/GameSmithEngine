#include "gtest/gtest.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/BufferLayout.h"

TEST(BufferLayoutTest, EmptyInitializerList) {
	GameSmith::BufferLayoutBuilder builder({});
	EXPECT_EQ(builder.GetElementCount(), (unsigned int)0);
	EXPECT_TRUE(builder.GetElements().empty());
}

TEST(BufferLayoutTest, SingleElement) {
	GameSmith::BufferLayoutBuilder builder({
		{"POSITION", GameSmith::ContainerDataType::Float3}
	});
	EXPECT_EQ(builder.GetElementCount(), (unsigned int)1);
	EXPECT_EQ(builder.GetElements()[0].SemanticName, "POSITION");
	EXPECT_EQ(builder.GetElements()[0].Type, GameSmith::ContainerDataType::Float3);
}

TEST(BufferLayoutTest, MultipleElements) {
	GameSmith::BufferLayoutBuilder builder({
		{"POSITION", GameSmith::ContainerDataType::Float3},
		{"NORMAL", GameSmith::ContainerDataType::Float3},
		{"TEXCOORD", GameSmith::ContainerDataType::Float2}
	});
	EXPECT_EQ(builder.GetElementCount(), (unsigned int)3);
	EXPECT_EQ(builder.GetElements()[0].SemanticName, "POSITION");
	EXPECT_EQ(builder.GetElements()[0].Type, GameSmith::ContainerDataType::Float3);
	EXPECT_EQ(builder.GetElements()[1].SemanticName, "NORMAL");
	EXPECT_EQ(builder.GetElements()[1].Type, GameSmith::ContainerDataType::Float3);
	EXPECT_EQ(builder.GetElements()[2].SemanticName, "TEXCOORD");
	EXPECT_EQ(builder.GetElements()[2].Type, GameSmith::ContainerDataType::Float2);
}

TEST(BufferLayoutTest, Iteration) {
	GameSmith::BufferLayoutBuilder builder({
		{"A", GameSmith::ContainerDataType::Int},
		{"B", GameSmith::ContainerDataType::Float}
	});
	int count = 0;
	for (auto& elem : builder) {
		EXPECT_FALSE(elem.SemanticName.empty());
		count++;
	}
	EXPECT_EQ(count, 2);
}
