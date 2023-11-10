#include "gtest/gtest.h"

#include "GameSmithEngine/EntitySystem/Components/Transform.h"

TEST(TransformTest, GetPosition) {
	GameSmith::Transform trams;
	EXPECT_EQ(trams.GetPosition(), glm::vec3(0, 0, 0));
}

TEST(TransformTest, SetPosition) {
	GameSmith::Transform trams;
	trams.SetPosition(glm::vec3(1, 2, 3));
	EXPECT_EQ(trams.GetPosition(), glm::vec3(1, 2, 3));
}

TEST(TransformTest, GetScale) {
	GameSmith::Transform trams;
	EXPECT_EQ(trams.GetScale(), glm::vec3(1, 1, 1));
}

TEST(TransformTest, SetScale) {
	GameSmith::Transform trams;
	trams.SetScale(glm::vec3(1, 2, 3));
	EXPECT_EQ(trams.GetScale(), glm::vec3(1, 2, 3));
}

TEST(TransformTest, GetRotation) {
	GameSmith::Transform trams;
	EXPECT_EQ(trams.GetRotation(), glm::vec3(0, 0, 0));
}

TEST(TransformTest, SetRotation) {
	GameSmith::Transform trams;
	trams.SetRotation(glm::vec3(1, 2, 3));
	EXPECT_EQ(trams.GetRotation(), glm::vec3(1, 2, 3));
}

TEST(TransformTest, IdentityModelMatrix) {
	GameSmith::Transform trams;
	glm::mat4 identity(1);
	EXPECT_EQ(trams.GetModelMatrix(), identity);
}

TEST(TransformTest, ComplicatedModelMatrix) {
	glm::vec3 translation(1, 2, 3);
	glm::vec3 scale(4, 5, 6);
	glm::vec3 rotation(7, 8, 9);

	GameSmith::Transform trams;
	trams.SetPosition(translation);
	trams.SetScale(scale);
	trams.SetRotation(rotation);

	glm::mat4 expectedMatrix = glm::transpose(glm::translate(glm::mat4(1), translation) *
		glm::scale(glm::mat4(1), scale) *
		glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1)));

	EXPECT_EQ(trams.GetModelMatrix(), expectedMatrix);
}