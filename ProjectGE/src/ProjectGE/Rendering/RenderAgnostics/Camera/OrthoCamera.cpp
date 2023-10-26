#include "gepch.h"
#include "OrthoCamera.h"

namespace ProjectGE {
	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float zNear, float zFar) : Camera(){
		m_Projection = glm::transpose(glm::orthoLH(left, right, bottom, top, zNear, zFar));
		UpdateMatrix();
	}
};
