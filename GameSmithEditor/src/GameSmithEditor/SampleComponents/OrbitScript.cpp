#include "OrbitScript.h"

namespace GameSmithEditor {
	void OrbitScript::OnUpdate(float dt)
	{
		m_t.SetPosition(m_Origin);
		m_Transform->SetPosition(glm::vec3(m_Radius, 0, 0));


		auto orbit = m_t.GetRotation();
		orbit += m_OribitSpeeds * dt * m_TimeScale;
		orbit = glm::mod(orbit, 2 * glm::pi<float>());
		m_t.SetRotation(orbit);
		auto localRot = m_Transform->GetRotation();
		localRot += m_LocalSpeeds * dt * m_TimeScale;
		localRot = glm::mod(localRot, 2 * glm::pi<float>());
		m_Transform->SetRotation(localRot);
	}
};

