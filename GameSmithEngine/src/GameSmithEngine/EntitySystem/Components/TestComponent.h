#pragma once
#include "Component.h"

namespace GameSmith {
	class GameChunkAsset;

	class GE_API TestComponent : public Component {
	public:
		TestComponent(GameObject* gameObject, Transform* transform);

		virtual void OnStart() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnDestroy() override;
		virtual int GetPriority() const override { return 0; };
		virtual std::string GetName() const override { return "TestComponent"; };
	private:
		float expose1 = 0;
		glm::vec2 expose2 = {};
		glm::vec3 expose3 = {};
		glm::vec4 expose4 = {};

		int expose5 = 0;
		int expose6[2] = {0};
		int expose7[3] = {0};
		int expose8[4] = {0};
		Connection<GameObject> m_TestRef;
		Connection<TestComponent> m_TestCompRef;
		Ref<GameChunkAsset> m_GameChunkAsset;
	};
};
