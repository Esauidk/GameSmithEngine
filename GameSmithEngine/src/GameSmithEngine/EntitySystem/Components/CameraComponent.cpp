#include "gepch.h"
#include "CameraComponent.h"
#include "GameSmithEngine/EntitySystem/Components/ComponentFactory.h"
#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"

namespace GameSmith {
	GE_REGISTERCOMPONENT(CameraComponent)

	CameraComponent* CameraComponent::s_ActiveCam = nullptr;

	CameraComponent::CameraComponent(GameObject* gameObject, Transform* transform) : Component(gameObject, transform), m_CamType(CameraType::Perspective) {
		if (s_ActiveCam == nullptr) {
			s_ActiveCam = this;
			m_OrthoCam.GetTransform().SetCoordinateFrame(transform);
			m_PerpCam.GetTransform().SetCoordinateFrame(transform);
		}
	}
	
	void CameraComponent::OnStart()
	{

	}

	void CameraComponent::OnUpdate(float dt)
	{
		auto renderPrep = EntityRenderPreparer::GetInstance();

		switch (m_CamType) {
		case Perspective:
		default:
			renderPrep->AddCamera({ &m_PerpCam });
			break;
		case Orthographic:
			renderPrep->AddCamera({ &m_OrthoCam });
			break;
		}
	}

	void CameraComponent::SetAsMainCamera()
	{
		s_ActiveCam = this;
		
		// TODO: Insert setting of camera to rendering manager
	}

	void CameraComponent::ChangeCameraType(CameraType camType)
	{
		m_CamType = camType;

		if (s_ActiveCam == this) {
			// TODO: Insert setting of camera to rendering manager
		}
	}
};