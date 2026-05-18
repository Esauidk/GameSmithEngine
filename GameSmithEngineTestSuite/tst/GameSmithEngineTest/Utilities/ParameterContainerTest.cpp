#include "gtest/gtest.h"
#include "GameSmithEngine/Utilities/ParameterContainer.h"

TEST(ParameterContainerTest, GetParameterSize) {
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Float), sizeof(glm::vec1));
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Float2), sizeof(glm::vec2));
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Float3), sizeof(glm::vec3));
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Float4), sizeof(glm::vec4));
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Int), sizeof(int));
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Int2), sizeof(int) * 2);
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Int3), sizeof(int) * 3);
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::Matrix), sizeof(glm::mat4));
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::CBuffer), (unsigned int)256);
	EXPECT_EQ(GameSmith::GetParameterSize(GameSmith::ContainerDataType::None), (unsigned int)0);
}

TEST(IntContainerTest, DefaultValue) {
	GameSmith::IntContainer c("test");
	EXPECT_EQ(c.GetData(), 0);
	EXPECT_EQ(c.GetType(), GameSmith::ContainerDataType::Int);
}

TEST(IntContainerTest, SetAndGet) {
	GameSmith::IntContainer c("test");
	c.SetData(42);
	EXPECT_EQ(c.GetData(), 42);
}

TEST(IntContainerTest, InitializedValue) {
	GameSmith::IntContainer c("test", 99);
	EXPECT_EQ(c.GetData(), 99);
}

TEST(IntContainerTest, MakeCopy) {
	GameSmith::IntContainer c("test", 77);
	auto copy = c.MakeCopy();
	EXPECT_EQ(copy->GetName(), "test");
	EXPECT_EQ(copy->GetType(), GameSmith::ContainerDataType::Int);
	EXPECT_EQ(((GameSmith::IntContainer*)copy.get())->GetData(), 77);
}

TEST(FloatContainerTest, DefaultValue) {
	GameSmith::FloatContainer c("test");
	EXPECT_EQ(c.GetData(), glm::vec1(0));
}

TEST(FloatContainerTest, SetAndGet) {
	GameSmith::FloatContainer c("test");
	glm::vec1 v(3.14f);
	c.SetData(v);
	EXPECT_EQ(c.GetData(), v);
}

TEST(Float2ContainerTest, SetAndGet) {
	GameSmith::Float2Container c("test");
	glm::vec2 v(1.0f, 2.0f);
	c.SetData(v);
	EXPECT_EQ(c.GetData(), v);
}

TEST(Float3ContainerTest, SetAndGet) {
	GameSmith::Float3Container c("test");
	glm::vec3 v(1.0f, 2.0f, 3.0f);
	c.SetData(v);
	EXPECT_EQ(c.GetData(), v);
}

TEST(MatrixContainerTest, DefaultValue) {
	GameSmith::MatrixContainor c("test");
	EXPECT_EQ(c.GetData(), glm::mat4(1));
}

TEST(CBufferContainerTest, DefaultValue) {
	GameSmith::CBufferContainer c("test");
	EXPECT_EQ(c.GetSize(), (unsigned int)256);
}

TEST(CBufferContainerTest, SetAndGet) {
	GameSmith::CBufferContainer c("test");
	char data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	c.SetData(data, 8);
	char* retrieved = c.GetCharData();
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(retrieved[i], data[i]);
	}
}

TEST(ConvertToParameterTest, IntConversion) {
	int val = 42;
	auto param = GameSmith::ConvertToParameter("test", GameSmith::ContainerDataType::Int, (const char*)&val);
	EXPECT_EQ(param->GetType(), GameSmith::ContainerDataType::Int);
	EXPECT_EQ(((GameSmith::IntContainer*)param.get())->GetData(), 42);
}

TEST(ConvertToParameterTest, FloatConversion) {
	float val = 3.14f;
	auto param = GameSmith::ConvertToParameter("test", GameSmith::ContainerDataType::Float, (const char*)&val);
	EXPECT_EQ(param->GetType(), GameSmith::ContainerDataType::Float);
}

TEST(ConvertToParameterTest, Vec4Conversion) {
	glm::vec4 val(1, 2, 3, 4);
	auto param = GameSmith::ConvertToParameter("test", GameSmith::ContainerDataType::Float4, (const char*)&val);
	EXPECT_EQ(param->GetType(), GameSmith::ContainerDataType::Float4);
	EXPECT_EQ(((GameSmith::Float4Containor*)param.get())->GetData(), val);
}

TEST(CreateContainerTest, AllTypes) {
	auto intC = GameSmith::CreateContainer("i", GameSmith::ContainerDataType::Int);
	EXPECT_EQ(intC->GetType(), GameSmith::ContainerDataType::Int);
	auto floatC = GameSmith::CreateContainer("f", GameSmith::ContainerDataType::Float);
	EXPECT_EQ(floatC->GetType(), GameSmith::ContainerDataType::Float);
	auto matC = GameSmith::CreateContainer("m", GameSmith::ContainerDataType::Matrix);
	EXPECT_EQ(matC->GetType(), GameSmith::ContainerDataType::Matrix);
	auto bufC = GameSmith::CreateContainer("b", GameSmith::ContainerDataType::CBuffer);
	EXPECT_EQ(bufC->GetType(), GameSmith::ContainerDataType::CBuffer);
}

TEST(ParameterContainerTest, GetCharData) {
	GameSmith::IntContainer c("test", 42);
	char* data = c.GetCharData();
	EXPECT_EQ(*(int*)data, 42);
}

TEST(ParameterContainerTest, GetFlags) {
	GameSmith::IntContainer c("test", 0, 7);
	EXPECT_EQ(c.GetFlags(), (unsigned int)7);
}
