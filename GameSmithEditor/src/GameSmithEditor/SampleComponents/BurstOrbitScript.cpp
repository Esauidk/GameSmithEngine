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
			renderer->SetMesh(GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\test.obj"));

			auto mat = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MaterialAsset>("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\ColorMat.mat");
			for (unsigned int i = 0; i < renderer->GetMaterialSlots(); i++) {
				renderer->SetMaterial(i, mat->CreateInstance());
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


