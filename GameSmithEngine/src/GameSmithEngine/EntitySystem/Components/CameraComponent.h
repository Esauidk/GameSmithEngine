#pragma once
#include "Component.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/OrthoCamera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/PerspectiveCamera.h"

namespace GameSmith {
	enum CameraType {
		Perspective,
		Orthographic
	};

	class GE_API CameraComponent : public Component {
	public:
		CameraComponent(GameObject* gameObject, Transform* transform);
		virtual void OnStart() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnDestroy() override {}
		virtual int GetPriority() const override { return 0; };
		virtual std::string GetName() const override { return "CameraComponent"; };
		void SetAsMainCamera();
		void ChangeCameraType(CameraType camType);
		Camera* GetCamera() {
			switch (m_CamType) {
			case Perspective:
			default:
				return &m_PerpCam;
			case Orthographic:
				return &m_OrthoCam;
			}
		}	
	private:
		CameraType m_CamType;
		OrthoCamera m_OrthoCam;
		PerspectiveCamera m_PerpCam;
	private:
		static CameraComponent* s_ActiveCam;
	};
};

