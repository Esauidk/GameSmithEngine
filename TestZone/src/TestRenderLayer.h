#include "EngineDependenciesOnly.h"

class TestRenderLayer : public ProjectGE::Layer {
public:
	// DIRECTX12 is ROW MAJOR
	struct test {
		glm::vec3 color;
	};

	TestRenderLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {}

private:
	ProjectGE::Ref<ProjectGE::VertexBuffer> vBuff;
	ProjectGE::Ref<ProjectGE::IndexBuffer> iBuff;
	ProjectGE::Ref<ProjectGE::ConstantBuffer> cBuff1;
	ProjectGE::Ref<ProjectGE::Material> m_Mat;
	ProjectGE::Ref<ProjectGE::ConstantBuffer> cBuff2;
	ProjectGE::Ref<ProjectGE::Shader> m_VShader;
	ProjectGE::Ref<ProjectGE::Shader> m_PShader;

	ProjectGE::Ref<ProjectGE::Sampler> m_Sampler;
	ProjectGE::Ref<ProjectGE::TextureAsset> m_Tex2d;
	Transform m_TriTrans;
	Transform m_SquareTrans;

	ProjectGE::OrthoCamera m_Cam;
	test m_Example1;
};