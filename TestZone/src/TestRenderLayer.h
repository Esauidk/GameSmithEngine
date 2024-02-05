#include "EngineDependenciesOnly.h"

class TestRenderLayer : public GameSmith::Layer {
public:
	TestRenderLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
private:
	GameSmith::Ref<GameSmith::VertexBuffer> vBuff;
	GameSmith::Ref<GameSmith::VertexBuffer> vBuff2;
	GameSmith::Ref<GameSmith::IndexBuffer> iBuff;
	GameSmith::Ref<GameSmith::GraphicsPipelineStateObject> mObj;
	GameSmith::Ref<GameSmith::ConstantBuffer> cBuff1;
	GameSmith::Ref<GameSmith::Material> m_Mat;
	GameSmith::Ref<GameSmith::Material> m_CopyMat;
	GameSmith::Ref<GameSmith::ConstantBuffer> cBuff2;
	GameSmith::Ref<GameSmith::Shader> m_VShader;
	GameSmith::Ref<GameSmith::Shader> m_PShader;

	GameSmith::Ref<GameSmith::Sampler> m_Sampler;
	GameSmith::Ref<GameSmith::TextureAsset> m_Tex2d;
	GameSmith::Transform m_TriTrans;
	GameSmith::Transform m_SquareTrans;
	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 lightColor;

	GameSmith::OrthoCamera m_Cam;
	GameSmith::PerspectiveCamera m_PerpCam;

	bool switchPerp = false;
	bool switchLight = false;
};