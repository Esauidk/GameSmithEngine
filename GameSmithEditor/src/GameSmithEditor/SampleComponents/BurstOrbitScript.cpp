#include "BurstOrbitScript.h"

#include "OrbitScript.h"

namespace GameSmithEditor {
	void BurstOrbitScript::OnUpdate(float dt)
	{
		if (m_NumObjs < 0) {
			m_NumObjs = 0;
		}

		auto gameObjManager = GameSmith::GameObjectManager::GetInstance();
		while (((int)m_Objs.size()) < m_NumObjs) {
			auto gameObj = gameObjManager->CreateGameObject();
			auto renderer = gameObj.lock()->AddComponent<GameSmith::MeshRenderer>().lock();
			renderer->SetMesh(GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>(GameSmith::ID(250309786, 50043, 18339, 17854270087279912363)));

			auto mat = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MaterialAsset>(GameSmith::ID(2652798047, 63418, 17489, 6150598651790595990));
			for (unsigned int i = 0; i < renderer->GetMaterialSlots(); i++) {
				renderer->SetMaterial(i, mat);
			}

			auto orbitScript = gameObj.lock()->AddComponent<GameSmithEditor::OrbitScript>();
			auto lockScript = orbitScript.lock();

			lockScript->SetOribitSpeeds(glm::vec3(ddist(rng), ddist(rng), ddist(rng)));
			lockScript->SetLocalRotationSpeed(glm::vec3(odist(rng), odist(rng), odist(rng)));
			lockScript->SetSphereSize(rdist(rng));

			m_Objs.push_back(gameObj);
		}

		while (((int)m_Objs.size()) > m_NumObjs) {
			auto obj = m_Objs.back();
			gameObjManager->DestroyGameObject(obj);
			m_Objs.pop_back();
		}

		for (auto obj : m_Objs) {
			auto orbitScript = obj.lock()->GetComponent<GameSmithEditor::OrbitScript>().lock();
			orbitScript->SetOrigin(m_Center);
			orbitScript->SetTimeScale(m_TimeScale);
		}
	}
};


