#include "gtest/gtest.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

TEST(BinaryStreamWriterTest, WriteAndGetBuffer) {
	GameSmith::BinaryStreamWriter writer(256);
	EXPECT_NE(writer.GetBuffer(), nullptr);
	EXPECT_EQ(writer.GetBufferSize(), (unsigned int)256);
}

TEST(BinaryStreamWriterTest, WriteUInt) {
	GameSmith::BinaryStreamWriter writer(64);
	writer.WriteUInt(42);
	char* cur = writer.GetCurPtr();
	EXPECT_EQ(*(unsigned int*)(cur - sizeof(unsigned int)), (unsigned int)42);
}

TEST(BinaryStreamWriterTest, WriteString) {
	std::string input = "hello";
	GameSmith::BinaryStreamWriter writer(128);
	writer.WriteString(input);
	EXPECT_STREQ(writer.GetBuffer().get(), input.c_str());
}

TEST(BinaryStreamWriterTest, WriteClass) {
	struct TestData { int x; float y; };
	TestData data{10, 3.14f};

	GameSmith::BinaryStreamWriter writer(64);
	writer.WriteClass<TestData>(&data);

	TestData* written = (TestData*)writer.GetBuffer().get();
	EXPECT_EQ(written->x, 10);
	EXPECT_FLOAT_EQ(written->y, 3.14f);
}

TEST(BinaryStreamWriterTest, ExternalBuffer) {
	char buffer[64] = {};
	GameSmith::BinaryStreamWriter writer(buffer, 64);
	EXPECT_EQ(writer.GetBuffer(), nullptr);
	writer.WriteUInt(99);
	EXPECT_EQ(*(unsigned int*)buffer, (unsigned int)99);
}

TEST(BinaryStreamWriterTest, MoveCurPtr) {
	GameSmith::BinaryStreamWriter writer(64);
	unsigned int start = writer.GetRemainingSpace();
	writer.MoveCurPtr(16);
	EXPECT_EQ(writer.GetRemainingSpace(), start - 16);
}

TEST(BinaryStreamWriterTest, RemainingSpace) {
	GameSmith::BinaryStreamWriter writer(32);
	EXPECT_EQ(writer.GetRemainingSpace(), (unsigned int)32);
	writer.WriteUInt(0);
	EXPECT_EQ(writer.GetRemainingSpace(), (unsigned int)(32 - sizeof(unsigned int)));
}

TEST(BinaryStreamReaderTest, ReadUInt) {
	char buffer[4];
	*(unsigned int*)buffer = 12345;
	GameSmith::BinaryStreamReader reader(buffer, 4);
	EXPECT_EQ(reader.GetUInt(), (unsigned int)12345);
}

TEST(BinaryStreamReaderTest, ReadString) {
	char buffer[16];
	memcpy(buffer, "world\0", 6);
	GameSmith::BinaryStreamReader reader(buffer, 16);
	EXPECT_EQ(reader.GetString(), "world");
}

TEST(BinaryStreamReaderTest, ReadClass) {
	struct TestData { int a; int b; };
	char buffer[sizeof(TestData)];
	TestData data{7, 8};
	memcpy(buffer, &data, sizeof(TestData));
	
	GameSmith::BinaryStreamReader reader(buffer, sizeof(TestData));
	TestData* read = reader.ReadClass<TestData>();
	EXPECT_EQ(read->a, 7);
	EXPECT_EQ(read->b, 8);
}

TEST(BinaryStreamReaderTest, GetBytes) {
	char buffer[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	GameSmith::BinaryStreamReader reader(buffer, 10);
	char* bytes = reader.GetBytes(5);
	for (int i = 0; i < 5; i++) {
		EXPECT_EQ(bytes[i], buffer[i]);
	}
}

TEST(BinaryStreamReaderTest, MoveForward) {
	char buffer[20] = {};
	GameSmith::BinaryStreamReader reader(buffer, 20);
	reader.MoveForward(10);
	EXPECT_EQ(reader.GetRemainingBytes(), (unsigned int)10);
}

TEST(BinaryStreamReaderTest, GetRemainingBytes) {
	char buffer[32] = {};
	GameSmith::BinaryStreamReader reader(buffer, 32);
	EXPECT_EQ(reader.GetRemainingBytes(), (unsigned int)32);
	reader.MoveForward(12);
	EXPECT_EQ(reader.GetRemainingBytes(), (unsigned int)20);
}

TEST(BinaryStreamReaderTest, GetCurPtr) {
	char buffer[8] = {};
	GameSmith::BinaryStreamReader reader(buffer, 8);
	EXPECT_EQ(reader.GetCurPtr(), buffer);
	reader.MoveForward(4);
	EXPECT_EQ(reader.GetCurPtr(), buffer + 4);
}

TEST(BinaryStreamRoundTripTest, WriteThenReadUInt) {
	GameSmith::BinaryStreamWriter writer(32);
	writer.WriteUInt(42);
	writer.WriteUInt(99);
	
	char* buf = writer.GetBuffer().get();
	GameSmith::BinaryStreamReader reader(buf, writer.GetBufferSize());
	EXPECT_EQ(reader.GetUInt(), (unsigned int)42);
	EXPECT_EQ(reader.GetUInt(), (unsigned int)99);
}

TEST(BinaryStreamRoundTripTest, WriteThenReadString) {
	GameSmith::BinaryStreamWriter writer(64);
	writer.WriteString("GameSmith");
	
	char* buf = writer.GetBuffer().get();
	GameSmith::BinaryStreamReader reader(buf, writer.GetBufferSize());
	EXPECT_EQ(reader.GetString(), "GameSmith");
}

TEST(BinaryStreamRoundTripTest, WriteThenReadClass) {
	struct Point { float x, y, z; };
	
	GameSmith::BinaryStreamWriter writer(32);
	Point p{1.0f, 2.0f, 3.0f};
	writer.WriteClass<Point>(&p);
	
	char* buf = writer.GetBuffer().get();
	GameSmith::BinaryStreamReader reader(buf, writer.GetBufferSize());
	Point* read = reader.ReadClass<Point>();
	EXPECT_FLOAT_EQ(read->x, 1.0f);
	EXPECT_FLOAT_EQ(read->y, 2.0f);
	EXPECT_FLOAT_EQ(read->z, 3.0f);
}

TEST(BinaryStreamRoundTripTest, WriteThenReadMultipleTypes) {
	GameSmith::BinaryStreamWriter writer(128);
	writer.WriteUInt(1);
	writer.WriteString("test");
	writer.WriteUInt(2);
	
	char* buf = writer.GetBuffer().get();
	GameSmith::BinaryStreamReader reader(buf, writer.GetBufferSize());
	EXPECT_EQ(reader.GetUInt(), (unsigned int)1);
	EXPECT_EQ(reader.GetString(), "test");
	EXPECT_EQ(reader.GetUInt(), (unsigned int)2);
}
