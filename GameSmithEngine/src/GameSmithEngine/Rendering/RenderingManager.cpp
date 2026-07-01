#include "gepch.h"
#include "RenderingManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12RendererAPI.h"
#include "GameSmithEngine/Rendering/Workflows/ForwardRender.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderPresetsFactory.h"

namespace GameSmith {
	static_assert(static_cast<int>(ShaderConstantType::Instance) == 3);

	GE_REGISTER_PRESET_SHADER(Instance, R"(
cbuffer Instance : register(b3)
{
    matrix M;
};
	)")

	RenderingManager* RenderingManager::s_Instance = nullptr;

	RenderingManager::RenderingManager()
	{
		m_RenderAPI = Scope<RendererAPI>(new DirectX12RendererAPI());
		m_RenderWorkflow = Scope<RenderWorkflow>(new ForwardRender(m_RenderAPI.get()));
		m_PSOManager = Scope<PipelineStateObjectManager>(new PipelineStateObjectManager(m_RenderAPI.get()));
		m_CameraBuffer = m_RenderAPI->CreateConstantBuffer(sizeof(CameraShaderData), "CameraData", UpdateFrequency::OncePerDrawCall);
		m_LightBuffer = m_RenderAPI->CreateConstantBuffer(sizeof(LightShaderData), "LightData", UpdateFrequency::OncePerDrawCall);
		m_ObjectBuffer = m_RenderAPI->CreateConstantBuffer(sizeof(ObjectData), "ObjectData", UpdateFrequency::OncePerDrawCall);
	}

	void RenderingManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new RenderingManager();
			GE_CORE_INFO("Rendering Manager Loaded!");
		}
	}

	void RenderingManager::SetRenderWorkflow(RenderWorkflow* workflow)
	{
		m_RenderWorkflow = Scope<RenderWorkflow>(workflow);
	}

	void RenderingManager::ShutDown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	void RenderingManager::SetShaderSourceCache(const std::unordered_map<std::string, ID>& shaderFiles)
	{
		for (auto& entry : shaderFiles) {
			m_ShaderIncludeCache.AddInclude(entry.first, entry.second);
		}
	}

	void RenderingManager::BeginScene(Camera* cam, LightSource* mainLight)
	{
		//m_RenderAPI->ClearCachedAssets();
		if (cam != nullptr) {
			CameraShaderData camData;
			camData.VP = cam->GetMatrix();
			camData.CameraWorldPos = cam->GetTransform().GetPosition();

			m_CameraData = camData;
			m_CameraBuffer->UpdateData((BYTE*)&m_CameraData, sizeof(m_CameraData));
		}

		LightShaderData lightData;
		if (mainLight != nullptr) {
			lightData.LightWorldPos = mainLight->GetLightVector();
			lightData.MainLightColor = mainLight->GetLightColor();	
		}
		else {
			lightData.LightWorldPos = glm::vec4(1, 0, 1, 0);
			lightData.MainLightColor = glm::vec3(0, 0, 0);
		}

		m_LightData = lightData;
		m_LightBuffer->UpdateData((BYTE*)&m_LightData, sizeof(m_LightData));
		
		m_RenderAPI->SetConstantBuffer(m_CameraBuffer, STAGE_VERTEX, ShaderConstantType::Camera);
		m_RenderAPI->SetConstantBuffer(m_LightBuffer, STAGE_VERTEX, ShaderConstantType::Light);

		m_RenderAPI->SetConstantBuffer(m_CameraBuffer, STAGE_PIXEL, ShaderConstantType::Camera);
		m_RenderAPI->SetConstantBuffer(m_LightBuffer, STAGE_PIXEL, ShaderConstantType::Light);
	}

	// TODO: RE-EXAMINE HOW ENDSCENE AND ENDFRAME ARE BEING CALLED
	void RenderingManager::EndScene()
	{
		m_RenderAPI->SubmitRecording();
	}

	void RenderingManager::EndFrame()
	{
		EndScene();
		m_RenderAPI->CompleteFrameSubmissions();
		m_PSOManager->CleanStateObjects();

		EndFrameRecordEvent e;
		m_FrameEndDispatch.Dispatch(e);
	}

	void RenderingManager::ClearTextures()
	{
		while (!m_ClearOnSwap.empty()) {
			auto tex = m_ClearOnSwap.front();
			m_ClearOnSwap.pop();

			tex->ClearTexture();
		}
	}

	void RenderingManager::Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat, Transform objectTransform)
	{
		ObjectData objData;
		objData.Model = objectTransform.GetModelMatrix();

		m_ObjectData = objData;
		m_ObjectBuffer->UpdateData((BYTE*)&m_ObjectData, sizeof(m_ObjectData));

		m_RenderAPI->SetConstantBuffer(m_ObjectBuffer, STAGE_VERTEX, ShaderConstantType::Instance);
		m_RenderAPI->SetConstantBuffer(m_ObjectBuffer, STAGE_PIXEL, ShaderConstantType::Instance);

		m_RenderWorkflow->Submit(vBuff, iBuff, mat);
		m_RenderAPI->SubmitRecording();
	}

	void RenderingManager::SyncDataTransfer()
	{
		m_RenderAPI->FlushDataTransfer();
	}

	Ref<Shader> RenderingManager::CompileOrRetrieveShader(const Stages stage, const char* shaderCode, unsigned int size, const ID& sourceAssetID)
	{
		if (m_CompiledShaderCache.contains(sourceAssetID)) {
			// Load asset file and return shader
		}

		unsigned int compiledSize;
		Ref<const char> compiledShader = m_RenderAPI->CompileShader(stage, shaderCode, size, "main", &m_ShaderIncludeCache, &compiledSize);

		return m_RenderAPI->LoadShader(compiledShader.get(), compiledSize);
	}
};
