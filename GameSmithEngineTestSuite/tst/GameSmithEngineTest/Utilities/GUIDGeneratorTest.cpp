#include "gtest/gtest.h"
#include "GameSmithEngine/Utilities/GUIDGenerator.h"

TEST(GUIDGeneratorTest, GenerateIDReturnsNonDefault) {
	GameSmith::ID id = GameSmith::GUIDGenerator::GenerateID();
	GameSmith::ID defaultId;
	EXPECT_NE(id, defaultId);
}

TEST(GUIDGeneratorTest, GeneratedIDsAreUnique) {
	GameSmith::ID id1 = GameSmith::GUIDGenerator::GenerateID();
	GameSmith::ID id2 = GameSmith::GUIDGenerator::GenerateID();
	EXPECT_NE(id1, id2);
}

TEST(GUIDGeneratorTest, IDEquality) {
	GameSmith::ID id1(1, 2, 3, 4);
	GameSmith::ID id2(1, 2, 3, 4);
	EXPECT_EQ(id1, id2);
}

TEST(GUIDGeneratorTest, IDInequality) {
	GameSmith::ID id1(1, 2, 3, 4);
	GameSmith::ID id2(5, 6, 7, 8);
	EXPECT_NE(id1, id2);
}

TEST(GUIDGeneratorTest, IDAssignment) {
	GameSmith::ID id1(1, 2, 3, 4);
	GameSmith::ID id2;
	id2 = id1;
	EXPECT_EQ(id1, id2);
}

TEST(GUIDGeneratorTest, IDObjectAutoGeneratesID) {
	GameSmith::IDObject obj;
	GameSmith::ID defaultId;
	EXPECT_NE(obj.GetID(), defaultId);
}

TEST(GUIDGeneratorTest, IDObjectSetAndGet) {
	GameSmith::IDObject obj;
	GameSmith::ID newId(99, 88, 77, 66);
	obj.SetID(newId);
	EXPECT_EQ(obj.GetID(), newId);
}

TEST(GUIDGeneratorTest, IDHasherProducesConsistentHash) {
	GameSmith::ID id(1, 2, 3, 4);
	IDHasher hasher;
	std::size_t h1 = hasher(id);
	std::size_t h2 = hasher(id);
	EXPECT_EQ(h1, h2);
}

TEST(GUIDGeneratorTest, IDStructDataAccess) {
	GameSmith::idData data{10, 20, 30, 40};
	GameSmith::ID id(data);
	auto retrieved = id.getData();
	EXPECT_EQ(retrieved.ID1, (unsigned int)10);
	EXPECT_EQ(retrieved.ID2, (unsigned short)20);
	EXPECT_EQ(retrieved.ID3, (unsigned short)30);
	EXPECT_EQ(retrieved.ID4, (unsigned long long)40);
}
