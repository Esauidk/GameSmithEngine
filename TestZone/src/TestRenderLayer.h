#include "EngineDependenciesOnly.h"

class TestRenderLayer : public ProjectGE::Layer {
public:
	TestRenderLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {}

private:
	ProjectGE::Ref<ProjectGE::VertexBuffer> vBuff;
	ProjectGE::Ref<ProjectGE::IndexBuffer> iBuff;
	ProjectGE::Ref<ProjectGE::ConstantBuffer> cBuff1;
	ProjectGE::Ref<ProjectGE::Material> m_Mat;
	ProjectGE::Ref<ProjectGE::Material> m_CopyMat;
	ProjectGE::Ref<ProjectGE::ConstantBuffer> cBuff2;
	ProjectGE::Ref<ProjectGE::Shader> m_VShader;
	ProjectGE::Ref<ProjectGE::Shader> m_PShader;

	ProjectGE::Ref<ProjectGE::Sampler> m_Sampler;
	ProjectGE::Ref<ProjectGE::TextureAsset> m_Tex2d;
	ProjectGE::Transform m_TriTrans;
	ProjectGE::Transform m_SquareTrans;
	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 lightColor;

	ProjectGE::OrthoCamera m_Cam;
	ProjectGE::PerspectiveCamera m_PerpCam;

	bool switchPerp = false;
	bool switchLight = false;
};