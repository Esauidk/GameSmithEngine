#include "gtest/gtest.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/Sources/PointLight.h"

TEST(PointLightTest, GetLightPosition) {
	GameSmith::PointLight light;
	EXPECT_EQ(light.GetLightVector(), glm::vec4(0, 0, 0, 1));
}

TEST(PointLightTest, GetLightColor) {
	GameSmith::PointLight light;
	EXPECT_EQ(light.GetLightColor(), glm::vec3(1, 1, 1));
}

TEST(PointLightTest, SetLightPosition) {
	GameSmith::PointLight light;
	glm::vec3 newPosition(5, 6, 7);
	light.SetLightPosition(newPosition);

	EXPECT_EQ(light.GetLightVector(), glm::vec4(5, 6, 7, 1));
}

TEST(PointLightTest, SetLightColor) {
	GameSmith::PointLight light;
	glm::vec3 newColor(10, 1, 2);
	light.SetLightColor(newColor);

	EXPECT_EQ(light.GetLightColor(), glm::vec3(10, 1, 2));
}