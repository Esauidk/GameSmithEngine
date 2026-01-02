#include "TestingLayer.h"

GE_REGISTERCOMPONENT(TestLayerComponent)

struct TestStruct {
	int val;
};

struct TestStruct2 {
	GameSmith::Ref<GameSmith::JobBatchCounter> waitMarker;
	int val;
};

void Test(GameSmith::JobStandardParamters jobParams, void* ptr) {
	TestStruct* t = static_cast<TestStruct*>(ptr);

	GE_APP_INFO("{0}, {1}", jobParams.batchIndex, t->val);
	GameSmith::WorkerCompleteCurrentJob();
}

void Test2(GameSmith::JobStandardParamters jobParams, void* ptr) {
	TestStruct2* t = static_cast<TestStruct2*>(ptr);

	GameSmith::WorkerPauseCurrentJob(t->waitMarker);
	GE_APP_INFO("{0}, {1}", jobParams.batchIndex ,t->val);
}

TestingLayer::TestingLayer() : GameSmith::Layer("Testing Layer")
{
	TestStruct st = { 5 };
	auto instance = GameSmith::JobManager::GetInstance();
	auto marker = instance->StartJobs(Test, &st, sizeof(TestStruct), 10, GameSmith::JobPriority::High);

	TestStruct2 st2 = { marker, 10 };
	instance->StartJobs(Test2, &st2, sizeof(TestStruct2), 1, GameSmith::JobPriority::High);

	/*auto id1 = GameSmith::ResourceManager::GetInstance()->ImportResource("C:\\Users\\esaus\\Documents\\Coding Projects\\test.obj");
	auto id2 = GameSmith::ResourceManager::GetInstance()->ImportResource("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso");
	auto id3 = GameSmith::ResourceManager::GetInstance()->ImportResource("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\RandomColorPS.cso");

	std::vector<std::pair<GameSmith::ID, GameSmith::Stages>> shaderIds;
	std::vector<std::pair<std::string, GameSmith::ID>> textureIds;
	std::vector<std::pair<std::string, GameSmith::ContainerDataType>> variables;

	shaderIds.push_back({ id2, GameSmith::Stages::STAGE_VERTEX });
	shaderIds.push_back({ id3, GameSmith::Stages::STAGE_PIXEL });

	variables.push_back({ "Model", GameSmith::ContainerDataType::Matrix });
	variables.push_back({ "Color1", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color2", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color3", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color4", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color5", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color6", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color7", GameSmith::ContainerDataType::Float4 });
	variables.push_back({ "Color8", GameSmith::ContainerDataType::Float4 });

	GameSmith::Ref<GameSmith::MaterialAsset> testMat = GameSmith::Ref<GameSmith::MaterialAsset>(new GameSmith::MaterialAsset(shaderIds, textureIds, variables));
	testMat->Serialize();

	GameSmith::ResourceManager::GetInstance()->WriteResource(testMat, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\TestFileMat.mat");

	int i = 0;
	*/

	auto gm = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	auto test = gm.lock()->AddComponent<GameSmith::TestComponent>().lock();

	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> variableEntries;
	test->GenerateVariableEntries(&variableEntries);

	auto entry = variableEntries.find("Test Variable1");
	glm::vec1 val;
	val.x = 10.0f;
	GameSmith::CastPtr<GameSmith::FloatContainer>(entry->second)->SetData(val);

	gm.lock()->GetTransform().lock()->SetPosition({ 2, 5, 1 });

	test->BootstrapVariableRegistry(variableEntries);


	auto gm1 = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	gm1.lock()->GetTransform().lock()->SetPosition({ 6, 8, 1 });

	/*std::vector<GameSmith::Connection<GameSmith::GameObject>> objs;
	objs.push_back(gm);
	objs.push_back(gm1);

	GameSmith::Ref<GameSmith::GameChunk> chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objs));
	GameSmith::Ref<GameSmith::GameChunkAsset> chunkAsset = GameSmith::Ref<GameSmith::GameChunkAsset>(new GameSmith::GameChunkAsset(chunk));
	auto id = GameSmith::ResourceManager::GetInstance()->WriteResource(chunkAsset, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\Test.chunk");
	auto asset = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::GameChunkAsset>(id);
	asset->GenerateInstance();
	i = 1;*/

	/*auto resourceMang = GameSmith::ResourceManager::GetInstance();
	resourceMang->SetAssetDirectory("Assets");
	resourceMang->ScanResources();

	auto con = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	auto compe = con.lock()->AddComponent<GameSmith::TestComponent>();
	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> varEntries;
	compe.lock()->GenerateVariableEntries(&varEntries);
	auto v1 = varEntries.find("Test Variable1");
	auto f1 = GameSmith::CastPtr<GameSmith::FloatContainer>(v1->second);
	glm::vec1 rawf1(1.0f);
	f1->SetData(rawf1);
	compe.lock()->BootstrapVariableRegistry(varEntries);

	auto con1 = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	auto comp = con1.lock()->AddComponent<TestLayerComponent>();

	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ConnectionContainer>> refEntries;
	comp.lock()->GenerateConnectionEntries(&refEntries);

	auto entry = refEntries.find("TestRef");
	entry->second->AssignRef(con);
	entry->second->AssignID(con.lock()->GetID());
	comp.lock()->BootstrapConnectionRegistry(refEntries);


	auto chunkMang = GameSmith::ChunkManager::GetInstance();
	auto ch = chunkMang->GetCurrentMainChunk();
	ch.lock()->AddObjectToChunk(con);
	ch.lock()->AddObjectToChunk(con1);

	auto fileLocation = std::format(
		"{0}\\{1}.{2}", 
		"C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone", 
		"TestChunk", 
		GameSmith::GameChunkAsset::GetStaticFileExtension()
	);

	auto chunkID = resourceMang->WriteResource(ch.lock(), fileLocation);
	chunkMang->LoadChunk(chunkID);
	auto loadedChunk = chunkMang->GetCurrentMainChunk();*/


	auto renderManager = GameSmith::RenderingManager::GetInstance();
	float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
	m_RenderTex = renderManager->GetRenderAPI()->CreateRenderTexture(GameSmith::Application::Get().GetWindow()->GetWidth(), GameSmith::Application::Get().GetWindow()->GetHeight(), color);
	GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_RenderTex.get()), false);
	renderManager->GetRenderAPI()->SetRenderTexture(m_RenderTex, 0);
	renderManager->SetFrameTexture(m_RenderTex);
}

void TestingLayer::OnImGuiRender()
{
}

void TestingLayer::OnUpdate(float dt)
{
	auto renderManager = GameSmith::RenderingManager::GetInstance();
	renderManager->SetForClear(m_RenderTex);
}
