#include <ProjectGE.h>

#include "imgui.h"

#include "ProjectGE/Rendering/DirectX12/Util/DirectX12RootSignatureBuilder.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12RootSignature.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12PipelineState.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12InputLayout.h"
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12TopologyResource.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12Texture2D.h"

#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Util.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12ShaderUtils.h"
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/SLab/SLab.h"
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public ProjectGE::Layer {
public:
	// DIRECTX12 is ROW MAJOR
	struct test {
		glm::vec3 color;
	};

	ExampleLayer() : Layer("Example"), m_Cam(-1.6f, 1.6f, -0.9f, 0.9f) {
		/* START: TEST CODE REMOVE */
		// Read Shaders(Vertex, Pixel)
		auto& core = ProjectGE::DirectX12Core::GetCore();
		auto device = core.GetDevice();

		char buffer[MAX_PATH] = { 0 };
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::string(buffer).find_last_of("\\");
		auto vertex = std::string(buffer).substr(0, pos).append("\\SampleVertexShader.cso");
		auto pixel = std::string(buffer).substr(0, pos).append("\\SamplePixelShader.cso");

		m_VShader = ProjectGE::RenderCommand::LoadShader(vertex);
		m_PShader = ProjectGE::RenderCommand::LoadShader(pixel);


		ProjectGE::VertexStruct triVertex[] = {
			{ {-0.5f, -0.5f, 0.0f}}, // 0
			{ {0.0f,  0.5f, 0.0f}}, // 1 
			{ {0.5f,  -0.5f, 0.0f}} // 2
		};

		ProjectGE::VertexStruct squareVertex[] = {
			{{-0.75f, -0.75f, 0.0f}, {0, 1}},
			{{0.75f, -0.75f, 0.0f}, {1, 1}},
			{{0.75f,  0.75f, 0.0f},{1, 0}},
			{{-0.75f,  0.75f, 0.0f}, {0, 0}}
		};

		vBuff = ProjectGE::RenderCommand::CreateVertexBuffer((BYTE*)&squareVertex, sizeof(ProjectGE::VertexStruct), _countof(squareVertex));

		ProjectGE::BufferLayoutBuilder layout = { {"POSITION", ProjectGE::ShaderDataType::Float3}, {"UV_TEXCOORD", ProjectGE::ShaderDataType::Float2} };

		vBuff->AttachLayout(layout);

		auto configuredLayout = vBuff->GetLayout();

		ProjectGE::PipelineStateInitializer init;
		init.shaders[ProjectGE::ShaderType::Vertex] = m_VShader;
		init.shaders[ProjectGE::ShaderType::Pixel] = m_PShader;
		init.toplopgyType = ProjectGE::TopologyType::Triangle;
		init.vertexLayout = configuredLayout;

		ProjectGE::RenderCommand::UpdatePipeline(init);
		ProjectGE::Application::Get().GetWindow().GetRenderer()->AttachContextResources();
		ProjectGE::RenderCommand::SetTopology(ProjectGE::TopologyType::Triangle);

		unsigned short indexCount[] = {
			0, 1, 2
		};


		WORD squareIndex[] = {
			2,1,0,0,3,2
		};

		iBuff = ProjectGE::RenderCommand::CreateIndexBuffer((unsigned short*)&squareIndex, _countof(squareIndex));

		ProjectGE::RenderCommand::SetVertexBuffer(vBuff);
		ProjectGE::RenderCommand::SetIndexBuffer(iBuff);
		
		ProjectGE::DirectX12Shader(ProjectGE::CompileShaderForDX12("struct VertexInput{float3 Position : POSITION;};struct VertexShaderOutput{float4 Position : SV_POSITION;};VertexShaderOutput main(VertexInput input){VertexShaderOutput vso;vso.Position = float4(input.Position, 1);return vso;}",
			"main", ProjectGE::STAGE_VERTEX, "test"));

		ProjectGE::SLabMetadata metadata;
		metadata.AddParameter(ProjectGE::ShaderParameter("Model", ProjectGE::ShaderDataType::Matrix));
		metadata.AddParameter(ProjectGE::ShaderParameter("InputColor", ProjectGE::ShaderDataType::Float3));

		// TODO: GET RID OF THIS, JUST FOR TESTING WITH SCENE DATA
		cBuff1 = ProjectGE::RenderCommand::CreateConstantBuffer(sizeof(ProjectGE::GloablShaderData));
		ProjectGE::RenderCommand::SetConstantBuffer(cBuff1, ProjectGE::STAGE_VERTEX, ProjectGE::ShaderConstantType::Global);

		cBuff2 = ProjectGE::RenderCommand::CreateConstantBuffer(metadata.GetByteSize());
		ProjectGE::RenderCommand::SetConstantBuffer(cBuff2, ProjectGE::STAGE_VERTEX, ProjectGE::ShaderConstantType::Instance);
		ProjectGE::RenderCommand::SetConstantBuffer(cBuff2, ProjectGE::STAGE_PIXEL, ProjectGE::ShaderConstantType::Instance);

		m_Sampler = ProjectGE::RenderCommand::CreateSampler(ProjectGE::FilterType::Point, ProjectGE::PaddingMethod::Clamp);
		ProjectGE::RenderCommand::SetSampler(m_Sampler, ProjectGE::STAGE_PIXEL);

		auto texture = std::string(buffer).substr(0, pos).append("\\test2.png");
		m_Tex2d = ProjectGE::RenderCommand::CreateTexture2D(texture);
		ProjectGE::RenderCommand::SetTexture2D(m_Tex2d, ProjectGE::STAGE_PIXEL);


		/* END: TEST CODE REMOVE */
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Tint Color", glm::value_ptr(m_Example1.color));
		ImGui::End();
	}


	void OnUpdate() override {
		float dt = ProjectGE::Application::Get().GetTimer().GetDeltaTimeSeconds();
		Transform& tf = m_Cam.GetTransform();

		glm::vec3 oldPos = tf.GetPosition();
		glm::vec3 TriOldPos = m_TriTrans.GetPosition();
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_A)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(-1, 0, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_D)) {
			glm::vec3 newPosition = oldPos + dt * glm::vec3(1, 0, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_W)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(0, 1, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_S)) {
			glm::vec3 newPosition = oldPos + dt* glm::vec3(0, -1, 0);
			tf.SetPosition(newPosition);
		}
		
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_R)) {
			glm::vec3 oldRotation = tf.GetRotation();
			tf.SetRotation(oldRotation + dt * glm::vec3(0,0,1));
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_J)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(-1, 0, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_L)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(1, 0, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_I)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, 1, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_K)) {
			glm::vec3 newPosition = TriOldPos + dt * glm::vec3(0, -1, 0);
			m_TriTrans.SetPosition(newPosition);
		}

		if (ProjectGE::Input::IsKeyPressed(GE_KEY_P)) {
			glm::vec3 oldRotation = m_TriTrans.GetRotation();
			m_TriTrans.SetRotation(oldRotation + dt * glm::vec3(0, 0, 1));
		}

		glm::mat4 tri = glm::transpose(m_TriTrans.GetModelMatrix());
		//glm::mat4 squ = glm::transpose(m_SquareTrans.GetModelMatrix());
		ProjectGE::GloablShaderData data;
		data.VP = m_Cam.GetMatrix();
		cBuff1->UpdateData((BYTE*)&data, sizeof(data));


		struct instanceData {
			glm::mat4 model;
			glm::vec3 color;
		} test;

		test.model = tri;
		test.color = { 1, 0, 1 };
		cBuff2->UpdateData((BYTE*)&test, sizeof(instanceData));

		//std::dynamic_pointer_cast<ProjectGE::DirectX12Texture2D>(m_Tex2d)->Test();

		ProjectGE::RenderCommand::DrawIndexed(iBuff->GetCount(), 1);
		ProjectGE::RenderCommand::SubmitRecording();
		//ProjectGE::Renderer::EndScene();
	}

	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {}

private:
	ProjectGE::Ref<ProjectGE::DirectX12PipelineStateData> refData;
	ProjectGE::Ref<ProjectGE::VertexBuffer> vBuff;
	ProjectGE::Ref<ProjectGE::IndexBuffer> iBuff;
	ProjectGE::Ref<ProjectGE::ConstantBuffer> cBuff1;
	ProjectGE::Ref<ProjectGE::ConstantBuffer> cBuff2;
	//ProjectGE::Ref<ProjectGE::PipelineStateObject> m_State;
	//ProjectGE::Ref<ProjectGE::ShaderArguementDefiner> m_Root;
	ProjectGE::Ref<ProjectGE::Shader> m_VShader;
	ProjectGE::Ref<ProjectGE::Shader> m_PShader;

	ProjectGE::Ref<ProjectGE::Sampler> m_Sampler;
	ProjectGE::Ref<ProjectGE::Texture2D> m_Tex2d;
	//ProjectGE::Ref<ProjectGE::GeometryPack> m_TriPack;
	Transform m_TriTrans;
	//ProjectGE::Ref<ProjectGE::GeometryPack> m_SquarePack;
	Transform m_SquareTrans;

	//ProjectGE::Ref<ProjectGE::ConstantBuffer> m_C;
	//ProjectGE::Ref<ProjectGE::ShaderArguement> m_Arg1;
	//ProjectGE::Ref<ProjectGE::ShaderArguement> m_Arg2;
	//ProjectGE::Ref<ProjectGE::ShaderArguement> m_Arg3;

	ProjectGE::OrthoCamera m_Cam;
	test m_Example1;
};
class TestZone : public ProjectGE::Application {
public:
	TestZone() {
		PushLayer(new ExampleLayer());
	}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new TestZone();
}
