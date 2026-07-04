#include "gtest/gtest.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/Camera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/OrthoCamera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/PerspectiveCamera.h"

static constexpr float TOL = 1e-5f;

// ---------------------------------------------------------------
// Orthographic Camera KAT
//
// DirectX LH Orthographic Off-Center (row-major after transpose):
//   2/(R-L)      0            0           (L+R)/(L-R)
//   0            2/(T-B)      0           (T+B)/(B-T)
//   0            0            1/(Zf-Zn)   Zn/(Zn-Zf)
//   0            0            0            1
//
// KAT values: L=-5, R=15, B=-10, T=20, Zn=1, Zf=101
//   2/(R-L) = 2/20       = 0.1
//   2/(T-B) = 2/30       ≈ 0.06666667
//   1/(Zf-Zn) = 1/100    = 0.01
//   (L+R)/(L-R) = 10/-20 = -0.5
//   (T+B)/(B-T) = 10/-30 ≈ -0.33333333
//   Zn/(Zn-Zf) = 1/-100  = -0.01
// ---------------------------------------------------------------
TEST(OrthoCameraKAT, KnownAnswer) {
	GameSmith::OrthoCamera cam(-5.0f, 15.0f, -10.0f, 20.0f, 1.0f, 101.0f);
	glm::mat4 m = cam.GetMatrix();

	EXPECT_NEAR(m[0][0], 0.1f, TOL);
	EXPECT_NEAR(m[0][1], 0.0f, TOL);
	EXPECT_NEAR(m[0][2], 0.0f, TOL);
	EXPECT_NEAR(m[0][3], 0.0f, TOL);

	EXPECT_NEAR(m[1][0], 0.0f, TOL);
	EXPECT_NEAR(m[1][1], 0.06666667f, TOL);
	EXPECT_NEAR(m[1][2], 0.0f, TOL);
	EXPECT_NEAR(m[1][3], 0.0f, TOL);

	EXPECT_NEAR(m[2][0], 0.0f, TOL);
	EXPECT_NEAR(m[2][1], 0.0f, TOL);
	EXPECT_NEAR(m[2][2], 0.01f, TOL);
	EXPECT_NEAR(m[2][3], 0.0f, TOL);

	EXPECT_NEAR(m[3][0], -0.5f, TOL);
	EXPECT_NEAR(m[3][1], -0.33333333f, TOL);
	EXPECT_NEAR(m[3][2], -0.01f, TOL);
	EXPECT_NEAR(m[3][3], 1.0f, TOL);
}

// ---------------------------------------------------------------
// Orthographic Camera KAT — symmetric volume (no translation)
//
// L=-1, R=1, B=-1, T=1, Zn=0, Zf=100
//   2/(R-L) = 2/2  = 1
//   2/(T-B) = 2/2  = 1
//   1/(Zf-Zn) = 1/100 = 0.01
//   (L+R)/(L-R) = 0/2 = 0
//   (T+B)/(B-T) = 0/2 = 0
//   Zn/(Zn-Zf) = 0/100 = 0
// ---------------------------------------------------------------
TEST(OrthoCameraKAT, SymmetricVolume) {
	GameSmith::OrthoCamera cam(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);
	glm::mat4 m = cam.GetMatrix();

	EXPECT_NEAR(m[0][0], 1.0f, TOL);
	EXPECT_NEAR(m[0][1], 0.0f, TOL);
	EXPECT_NEAR(m[0][2], 0.0f, TOL);
	EXPECT_NEAR(m[0][3], 0.0f, TOL);

	EXPECT_NEAR(m[1][0], 0.0f, TOL);
	EXPECT_NEAR(m[1][1], 1.0f, TOL);
	EXPECT_NEAR(m[1][2], 0.0f, TOL);
	EXPECT_NEAR(m[1][3], 0.0f, TOL);

	EXPECT_NEAR(m[2][0], 0.0f, TOL);
	EXPECT_NEAR(m[2][1], 0.0f, TOL);
	EXPECT_NEAR(m[2][2], 0.01f, TOL);
	EXPECT_NEAR(m[2][3], 0.0f, TOL);

	EXPECT_NEAR(m[3][0], 0.0f, TOL);
	EXPECT_NEAR(m[3][1], 0.0f, TOL);
	EXPECT_NEAR(m[3][2], 0.0f, TOL);
	EXPECT_NEAR(m[3][3], 1.0f, TOL);
}

// ---------------------------------------------------------------
// Perspective Camera KAT
//
// DirectX LH Perspective FOV (row-major after transpose):
//   cot(FOV/2)/aspect     0        0               0
//   0                     cot(FOV/2) 0               0
//   0                     0        Zf/(Zf-Zn)      -Zn*Zf/(Zf-Zn)
//   0                     0        1               0
//
// KAT values: w=1920, h=1080, FOV=60°, Zn=0.2, Zf=500
//   cot(30°) = √3 ≈ 1.7320508075688772
//   aspect = 1920/1080 = 16/9 ≈ 1.7777777777777778
//   cotHF/aspect ≈ 0.9742785792574934
//   Zf/(Zf-Zn) = 500/499.8 ≈ 1.0004001600640256
//   -Zn*Zf/(Zf-Zn) = -100/499.8 ≈ -0.20008003201280513
// ---------------------------------------------------------------
TEST(PerspectiveCameraKAT, KnownAnswer) {
	GameSmith::PerspectiveCamera cam(1920.0f, 1080.0f, glm::radians(60.0f), 0.2f, 500.0f);
	glm::mat4 m = cam.GetMatrix();

	EXPECT_NEAR(m[0][0], 0.97427858f, TOL);
	EXPECT_NEAR(m[0][1], 0.0f, TOL);
	EXPECT_NEAR(m[0][2], 0.0f, TOL);
	EXPECT_NEAR(m[0][3], 0.0f, TOL);

	EXPECT_NEAR(m[1][0], 0.0f, TOL);
	EXPECT_NEAR(m[1][1], 1.73205081f, TOL);
	EXPECT_NEAR(m[1][2], 0.0f, TOL);
	EXPECT_NEAR(m[1][3], 0.0f, TOL);

	EXPECT_NEAR(m[2][0], 0.0f, TOL);
	EXPECT_NEAR(m[2][1], 0.0f, TOL);
	EXPECT_NEAR(m[2][2], 1.00040016f, TOL);
	EXPECT_NEAR(m[2][3], 1.0f, TOL);

	EXPECT_NEAR(m[3][0], 0.0f, TOL);
	EXPECT_NEAR(m[3][1], 0.0f, TOL);
	EXPECT_NEAR(m[3][2], -0.20008003f, TOL);
	EXPECT_NEAR(m[3][3], 0.0f, TOL);
}

// ---------------------------------------------------------------
// Perspective Camera KAT — 90° FOV for simpler trig
//
// FOV=90°, cot(45°)=1, w=1920, h=1080, Zn=1, Zf=100
//   element(0,0) = 1 / (1920/1080) = 1080/1920 = 0.5625
//   element(1,1) = 1
//   element(2,2) = 100/(100-1) = 100/99 ≈ 1.01010101
//   element(3,2) = -1*100/(100-1) = -100/99 ≈ -1.01010101
// ---------------------------------------------------------------
TEST(PerspectiveCameraKAT, NinetyDegreeFOV) {
	GameSmith::PerspectiveCamera cam(1920.0f, 1080.0f, glm::radians(90.0f), 1.0f, 100.0f);
	glm::mat4 m = cam.GetMatrix();

	EXPECT_NEAR(m[0][0], 0.5625f, TOL);
	EXPECT_NEAR(m[0][1], 0.0f, TOL);
	EXPECT_NEAR(m[0][2], 0.0f, TOL);
	EXPECT_NEAR(m[0][3], 0.0f, TOL);

	EXPECT_NEAR(m[1][0], 0.0f, TOL);
	EXPECT_NEAR(m[1][1], 1.0f, TOL);
	EXPECT_NEAR(m[1][2], 0.0f, TOL);
	EXPECT_NEAR(m[1][3], 0.0f, TOL);

	float zz = 100.0f / 99.0f;
	EXPECT_NEAR(m[2][0], 0.0f, TOL);
	EXPECT_NEAR(m[2][1], 0, TOL);
	EXPECT_NEAR(m[2][2], zz, TOL);
	EXPECT_NEAR(m[2][3], 1.0f, TOL);

	EXPECT_NEAR(m[3][0], 0.0f, TOL);
	EXPECT_NEAR(m[3][1], 0.0f, TOL);
	EXPECT_NEAR(m[3][2], -zz, TOL);
	EXPECT_NEAR(m[3][3], 0.0f, TOL);
}

// ---------------------------------------------------------------
// Camera base class: VP = Projection * View
// With identity transform, View = I, so VP = Projection.
// Setting position should change the view matrix.
// ---------------------------------------------------------------
TEST(CameraTest, IdentityTransformGivesProjectionOnly) {
	GameSmith::PerspectiveCamera cam(1920.0f, 1080.0f, glm::radians(60.0f), 0.2f, 500.0f);
	glm::mat4 identity(1.0f);
	EXPECT_NE(cam.GetMatrix(), identity);
}

TEST(CameraTest, CameraTransformAffectsMatrix) {
	GameSmith::OrthoCamera cam(-1, 1, -1, 1);
	glm::mat4 before = cam.GetMatrix();
	cam.GetTransform().SetPosition(glm::vec3(10, 0, 0));
	glm::mat4 after = cam.GetMatrix();
	EXPECT_NE(before, after);
}

TEST(PerspectiveCameraTest, SetWidthChangesMatrix) {
	GameSmith::PerspectiveCamera cam(1920.0f, 1080.0f, glm::radians(60.0f), 0.2f, 500.0f);
	glm::mat4 before = cam.GetMatrix();
	cam.SetWidth(1280.0f);
	glm::mat4 after = cam.GetMatrix();
	EXPECT_NE(before, after);
}

TEST(PerspectiveCameraTest, SetHeightChangesMatrix) {
	GameSmith::PerspectiveCamera cam(1920.0f, 1080.0f, glm::radians(60.0f), 0.2f, 500.0f);
	glm::mat4 before = cam.GetMatrix();
	cam.SetHeight(720.0f);
	glm::mat4 after = cam.GetMatrix();
	EXPECT_NE(before, after);
}

TEST(PerspectiveCameraTest, SetFOVChangesMatrix) {
	GameSmith::PerspectiveCamera cam(1920.0f, 1080.0f, glm::radians(60.0f), 0.2f, 500.0f);
	glm::mat4 before = cam.GetMatrix();
	cam.SetFOV(glm::radians(45.0f));
	glm::mat4 after = cam.GetMatrix();
	EXPECT_NE(before, after);
}
