#include "gtest/gtest.h"
#include "GameSmithEngine/Rendering/PipelineStateObjectManager.h"

namespace {
	class MockPSO : public GameSmith::GraphicsPipelineStateObject {
	public:
		MockPSO(int id) : m_Id(id) {}
		int m_Id;
	};

	class MockRendererAPI : public GameSmith::RendererAPI {
	public:
		int nextPsoId = 0;
		MockRendererAPI() {}

		virtual void Clear() override {}
		virtual void SetClearColor(const glm::vec4& color) override {}
		virtual void DrawIndexed(UINT indecies, UINT instances) override {}

		virtual GameSmith::Ref<GameSmith::VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) override { return nullptr; }
		virtual void SetVertexBuffer(GameSmith::Ref<GameSmith::VertexBuffer> vbuffer) override {}
		virtual GameSmith::Ref<GameSmith::IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) override { return nullptr; }
		virtual void SetIndexBuffer(GameSmith::Ref<GameSmith::IndexBuffer> ibuffer) override {}

		virtual GameSmith::Ref<GameSmith::Shader> LoadShader(std::string path) override { return nullptr; }
		virtual GameSmith::Ref<GameSmith::Shader> LoadShader(const char* byteCode, unsigned int length) override { return nullptr; }
		virtual GameSmith::Ref<const char> CompileShader(const GameSmith::Stages stage, const char* rawCode, const unsigned int length, const char* entryPt, GameSmith::ShaderIncludeCache* includeCache, unsigned int* outSize) override { return nullptr; }

		virtual GameSmith::Ref<GameSmith::ConstantBuffer> CreateConstantBuffer(UINT size, std::string name) override { return nullptr; }
		virtual GameSmith::Ref<GameSmith::ConstantBuffer> CreateConstantBuffer(UINT size) override { return nullptr; }
		virtual void SetConstantBuffer(GameSmith::Ref<GameSmith::ConstantBuffer> cbuffer, GameSmith::Stages stage, GameSmith::ShaderConstantType constantType) override {}

		virtual GameSmith::Ref<GameSmith::Texture2D> CreateTexture2D(char* data, UINT size) override { return nullptr; }
		virtual GameSmith::Ref<GameSmith::RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height) override { return nullptr; }
		virtual GameSmith::Ref<GameSmith::RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height, float* clearColor) override { return nullptr; }
		virtual void SetRenderTexture(GameSmith::Ref<GameSmith::RenderTexture> rt, UINT index) override {}
		virtual void SetTexture2D(GameSmith::Ref<GameSmith::Texture2D> tex, UINT slot, GameSmith::Stages stage) override {}

		virtual GameSmith::Ref<GameSmith::Sampler> CreateSampler(GameSmith::FilterType img, GameSmith::PaddingMethod padMode) override { return nullptr; }
		virtual void SetSampler(GameSmith::Ref<GameSmith::Sampler> sampler, GameSmith::Stages stage) override {}

		virtual void SetTopology(GameSmith::TopologyType& type, bool tesselation) override {}

		virtual GameSmith::Ref<GameSmith::GraphicsPipelineStateObject> CreateGraphicsPipelineState(GameSmith::PipelineStateInitializer& init) override {
			return GameSmith::Ref<GameSmith::GraphicsPipelineStateObject>(new MockPSO(nextPsoId++));
		}
		virtual void SetGraphicsPipelineState(GameSmith::Ref<GameSmith::GraphicsPipelineStateObject> pso) override {}

		virtual void SubmitRecording() override {}
		virtual void FlushDataTransfer() override {}
		virtual void CompleteFrameSubmissions() override {}
		virtual void ClearCachedAssets() override {}

		virtual GameSmith::RendererAPI::API GetAPI() override { return GameSmith::RendererAPI::API::None; }
	};
}

class PipelineStateObjectManagerTest : public ::testing::Test {
protected:
	MockRendererAPI* mockApi;
	GameSmith::PipelineStateObjectManager* manager;

	void SetUp() override {
		mockApi = new MockRendererAPI();
		manager = new GameSmith::PipelineStateObjectManager(mockApi);
	}

	void TearDown() override {
		delete manager;
		delete mockApi;
	}
};

TEST_F(PipelineStateObjectManagerTest, CreateNewPSO) {
	GameSmith::PipelineStateInitializer init;
	auto pso = manager->RetrieveOrCreateStateObject(init);
	EXPECT_NE(pso.get(), nullptr);
}

TEST_F(PipelineStateObjectManagerTest, RetrieveFromCache) {
	GameSmith::PipelineStateInitializer init;
	auto first = manager->RetrieveOrCreateStateObject(init);
	auto second = manager->RetrieveOrCreateStateObject(init);
	auto testPSOFirst = GameSmith::CastPtr<MockPSO>(first);
	auto testPSOSecond = GameSmith::CastPtr<MockPSO>(second);
	EXPECT_EQ(testPSOFirst->m_Id, testPSOSecond->m_Id);
}

TEST_F(PipelineStateObjectManagerTest, DifferentInitsCreateDifferentPSOs) {
	GameSmith::PipelineStateInitializer a;
	GameSmith::PipelineStateInitializer b;
	b.tesselation = true;
	auto psoA = manager->RetrieveOrCreateStateObject(a);
	auto psoB = manager->RetrieveOrCreateStateObject(b);
	auto testPSOFirst = GameSmith::CastPtr<MockPSO>(psoA);
	auto testPSOSecond = GameSmith::CastPtr<MockPSO>(psoB);
	EXPECT_NE(testPSOFirst->m_Id, testPSOSecond->m_Id);
}

TEST_F(PipelineStateObjectManagerTest, CleanMovesToCurrent) {
	GameSmith::PipelineStateInitializer init;
	auto pso = manager->RetrieveOrCreateStateObject(init);
	manager->CleanStateObjects();
	// After clean, the current map switches and old becomes cleanup
	// Access again should find it in the new current (from cleanup map)
	auto psoAgain = manager->RetrieveOrCreateStateObject(init);
	auto testPSOFirst = GameSmith::CastPtr<MockPSO>(pso);
	auto testPSOSecond = GameSmith::CastPtr<MockPSO>(psoAgain);
	EXPECT_EQ(testPSOFirst->m_Id, testPSOSecond->m_Id);
}

TEST_F(PipelineStateObjectManagerTest, CleanRemovesUnreferencedPSOs) {
	GameSmith::PipelineStateInitializer init;
	int oldId = 0;
	{
		auto pso = manager->RetrieveOrCreateStateObject(init);
		oldId = GameSmith::CastPtr<MockPSO>(pso)->m_Id;
	}
	// Only this manager holds a ref now
	manager->CleanStateObjects();
	// TODO: Rework how cleanup works
	manager->CleanStateObjects();
	// After clean, the PSO should be removed from cleanup map since use_count == 1
	// Then when we retrieve again it should create a new one
	auto pso2 = manager->RetrieveOrCreateStateObject(init);
	auto testPSO = GameSmith::CastPtr<MockPSO>(pso2);
	EXPECT_NE(testPSO->m_Id, oldId);
}

TEST_F(PipelineStateObjectManagerTest, MultipleUniquePSOs) {
	GameSmith::PipelineStateInitializer specs[3];
	specs[0].numRT = 1;
	specs[1].numRT = 2;
	specs[2].numRT = 3;
	
	auto pso0 = manager->RetrieveOrCreateStateObject(specs[0]);
	auto pso1 = manager->RetrieveOrCreateStateObject(specs[1]);
	auto pso2 = manager->RetrieveOrCreateStateObject(specs[2]);
	
	EXPECT_NE(pso0.get(), pso1.get());
	EXPECT_NE(pso1.get(), pso2.get());
	EXPECT_NE(pso0.get(), pso2.get());
	
	// Each should cache and return same object
	EXPECT_EQ(manager->RetrieveOrCreateStateObject(specs[0]).get(), pso0.get());
	EXPECT_EQ(manager->RetrieveOrCreateStateObject(specs[1]).get(), pso1.get());
	EXPECT_EQ(manager->RetrieveOrCreateStateObject(specs[2]).get(), pso2.get());
}
