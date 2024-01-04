#include "TestRenderLayer.h"

#include "imgui.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/SLab/SLab.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/DirectX12VertexBuffer.h"

TestRenderLayer::TestRenderLayer() : Layer("TestRender"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f), m_PerpCam(glm::pi<float>() / 3, (float)GameSmith::Application::Get().GetWindow().GetWidth(), (float)GameSmith::Application::Get().GetWindow().GetHeight()) {
	char buffer[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::string(buffer).find_last_of("\\");
	auto vertex = std::string(buffer).substr(0, pos).append("\\SampleVertexShader.cso");
	auto pixel = std::string(buffer).substr(0, pos).append("\\BlinnPhongPixelShader.cso");
	auto hull = std::string(buffer).substr(0, pos).append("\\BasicSubdivideHS.cso");
	auto domain = std::string(buffer).substr(0, pos).append("\\BasicRenderDS.cso");

	GameSmith::RendererAPI* renderAPI = GameSmith::RenderingManager::GetInstance()->GetRenderAPI();

	GameSmith::VertexStruct test1[] = {
		{{0, 0, 0}, {0, 1}, {0, 0, 0}},
		{{1, 1, 1}, {1, 1}, {1, 1, 1}}
	};

	GameSmith::VertexStruct test2[] = {
		{{1, 1, 1}, {0, 1}, {0, 0, 0}},
		{{1, 1, 1}, {1, 1}, {1, 1, 1}}
	};
	unsigned int index[] = {
		0, 1, 0
	};

	vBuff = renderAPI->CreateVertexBuffer((BYTE*)test1, sizeof(test1), _countof(test1));
	vBuff2 = renderAPI->CreateVertexBuffer((BYTE*)test2, sizeof(test2), _countof(test2));
	iBuff = renderAPI->CreateIndexBuffer(index, _countof(index));

	m_VShader = renderAPI->LoadShader(vertex);
	m_PShader = renderAPI->LoadShader(pixel);

	GameSmith::Ref<GameSmith::ShaderAsset> shaderV(new GameSmith::ShaderAsset(m_VShader));
	GameSmith::Ref<GameSmith::ShaderAsset> shaderP(new GameSmith::ShaderAsset(m_PShader));

	GameSmith::PipelineStateInitializer init;
	init.shaderSet.shaders[GameSmith::STAGE_VERTEX] = shaderV;
	init.shaderSet.shaders[GameSmith::STAGE_PIXEL] = shaderP;
	init.toplopgyType = GameSmith::TopologyType::Triangle;
	init.tesselation = false;

	mObj = renderAPI->CreateGraphicsPipelineState(init);
	auto hullShader = renderAPI->LoadShader(hull);
	auto domainShader = renderAPI->LoadShader(domain);
}

void TestRenderLayer::OnImGuiRender() {
	ImGui::Begin("Camera Setting");
	ImGui::Checkbox("Perspective Camera", &switchPerp);
	ImGui::Separator();
	ImGui::Text("Light Settings");
	ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));
	ImGui::Checkbox("Point Light", &switchLight);
	ImGui::InputFloat3("Direction Light: Direction", glm::value_ptr(lightDir));
	ImGui::InputFloat3("Point Light: Position", glm::value_ptr(lightPos));
	ImGui::End();
}

void TestRenderLayer::OnUpdate() {
	float dt = GameSmith::Application::Get().GetTimer().GetDeltaTimeSeconds();
	GameSmith::Transform& tf = m_Cam.GetTransform();
	GameSmith::Transform& perTf = m_PerpCam.GetTransform();

	glm::vec3 oldPos = tf.GetPosition();
	glm::vec3 TriOldPos = m_TriTrans.GetPosition();
	if (GameSmith::Input::IsKeyPressed(GE_KEY_A)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(-1, 0, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_D)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(1, 0, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_W)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(0, 1, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_S)) {
		glm::vec3 newPosition = oldPos + dt * glm::vec3(0, -1, 0);
		tf.SetPosition(newPosition);
		perTf.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_R)) {
		glm::vec3 oldRotation = tf.GetRotation();
		tf.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_J)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(-1, 0, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_L)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(1, 0, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_I)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 1, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_K)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, -1, 0);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_O)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 0, 1);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_P)) {
		glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 0, -1);
		m_TriTrans.SetPosition(newPosition);
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_M)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_N)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 1, 0));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_B)) {
		glm::vec3 oldRotation = m_TriTrans.GetRotation();
		m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, -1, 0));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_Z)) {
		glm::vec3 oldScale = m_TriTrans.GetScale();
		m_TriTrans.SetScale(oldScale + dt * glm::vec3(-1, -1, -1));
	}

	if (GameSmith::Input::IsKeyPressed(GE_KEY_X)) {
		glm::vec3 oldScale = m_TriTrans.GetScale();
		m_TriTrans.SetScale(oldScale + dt * glm::vec3(1, 1, 1));
	}

	glm::mat4 tri = m_TriTrans.GetModelMatrix();
	//glm::mat4 squ = glm::transpose(m_SquareTrans.GetModelMatrix());

	//std::dynamic_pointer_cast<GameSmith::DirectX12Texture2D>(m_Tex2d)->Test();

	auto renderManager = GameSmith::RenderingManager::GetInstance();
	auto renderAPI = renderManager->GetRenderAPI();

	GameSmith::DirectionalLight light;
	light.SetLightColor(lightColor);
	light.SetLightDirection(lightDir);

	GameSmith::PointLight pLight;
	pLight.SetLightPosition(lightPos);
	pLight.SetLightColor(lightColor);

	if (!switchPerp) {
		if (!switchLight) {
			renderManager->BeginScene(&m_Cam, &light);
		}
		else {
			renderManager->BeginScene(&m_Cam, &pLight);
		}
	}
	else {
		if (!switchLight) {
			renderManager->BeginScene(&m_PerpCam, &light);
		}
		else {
			renderManager->BeginScene(&m_PerpCam, &pLight);
		}
		
	}

	renderAPI->SetGraphicsPipelineState(mObj);
	renderAPI->SetIndexBuffer(iBuff);
	GameSmith::TopologyType type = GameSmith::TopologyType::Triangle;
	renderAPI->SetTopology(type, false);

	auto& core = GameSmith::DirectX12Core::GetCore();
	core.GetDirectCommandContext().GetStateManager().BindState();
	D3D12_VERTEX_BUFFER_VIEW view = GameSmith::CastPtr<GameSmith::DirectX12VertexBuffer>(vBuff)->GenerateView();
	D3D12_VERTEX_BUFFER_VIEW view1 = GameSmith::CastPtr<GameSmith::DirectX12VertexBuffer>(vBuff2)->GenerateView();
	core.GetDirectCommandContext().GetCommandList()->IASetVertexBuffers(0, 1, &view);
	core.GetDirectCommandContext().GetCommandList()->DrawIndexedInstanced(3, 1, 0, 0, 0);

	core.GetDirectCommandContext().GetCommandList()->IASetVertexBuffers(0, 1, &view1);
	core.GetDirectCommandContext().GetCommandList()->DrawIndexedInstanced(3, 1, 0, 0, 0);
	
	renderAPI->SubmitRecording();
	
}