#include "gtest/gtest.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/Sources/DirectionalLight.h"

TEST(DirectionalLightTest, GetLightPosition) {
	GameSmith::DirectionalLight light;
	EXPECT_EQ(light.GetLightVector(), glm::vec4(0, 0, 0, 0));
}

TEST(DirectionalLightTest, GetLightColor) {
	GameSmith::DirectionalLight light;
	EXPECT_EQ(light.GetLightColor(), glm::vec3(1, 1, 1));
}

TEST(DirectionalLightTest, SetLightDirection) {
	GameSmith::DirectionalLight light;
	glm::vec3 newPosition(5, 6, 7);
	light.SetLightDirection(newPosition);

	EXPECT_EQ(light.GetLightVector(), glm::vec4(5, 6, 7, 0));
}

TEST(DirectionalLightTest, SetLightColor) {
	GameSmith::DirectionalLight light;
	glm::vec3 newColor(10, 1, 2);
	light.SetLightColor(newColor);

	EXPECT_EQ(light.GetLightColor(), glm::vec3(10, 1, 2));
}