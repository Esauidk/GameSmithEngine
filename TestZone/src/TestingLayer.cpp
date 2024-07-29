#include "TestingLayer.h"

TestingLayer::TestingLayer() : GameSmith::Layer("Testing Layer")
{

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

	auto gm = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	auto test = gm.lock()->AddComponent<GameSmith::TestComponent>().lock();

	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> variableEntries;
	test->GenerateVariableEntries(&variableEntries);

	auto entry = variableEntries.find("Test Variable1");
	glm::vec1 val;
	val.x = 10.0f;
	GameSmith::CastPtr<GameSmith::FloatContainer>(entry->second)->SetData(val);

	gm.lock()->GetTransform().lock()->SetPosition({ 2, 5, 1 });

	test->BootstrapRegistry(variableEntries);


	auto gm1 = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	gm1.lock()->GetTransform().lock()->SetPosition({ 6, 8, 1 });

	std::vector<GameSmith::Connection<GameSmith::GameObject>> objs;
	objs.push_back(gm);
	objs.push_back(gm1);

	GameSmith::Ref<GameSmith::GameChunk> chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objs));
	GameSmith::Ref<GameSmith::GameChunkAsset> chunkAsset = GameSmith::Ref<GameSmith::GameChunkAsset>(new GameSmith::GameChunkAsset(chunk));
	auto id = GameSmith::ResourceManager::GetInstance()->WriteResource(chunkAsset, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\Test.chunk");
	auto asset = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::GameChunkAsset>(id);
	asset->GenerateInstance();
	i = 1;*/

	auto resourceMang = GameSmith::ResourceManager::GetInstance();
	resourceMang->SetAssestDirectory("Assets");
	resourceMang->ScanResources();

	auto clib = GameSmith::ContentLibraryManager::GetInstance();
	clib->LoadContentLibrary("GameProject.dll");

	auto con = GameSmith::GameObjectManager::GetInstance()->CreateGameObject();
	con.lock()->AddComponent("FakeComp");

	auto renderManager = GameSmith::RenderingManager::GetInstance();
	float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
	m_RenderTex = renderManager->GetRenderAPI()->CreateRenderTexture((float)GameSmith::Application::Get().GetWindow()->GetWidth(), (float)GameSmith::Application::Get().GetWindow()->GetHeight(), color);
	GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_RenderTex.get()), false);
	renderManager->GetRenderAPI()->SetRenderTexture(m_RenderTex, 0);
	renderManager->SetFrameTexture(m_RenderTex);

	auto chunkMang = GameSmith::ChunkManager::GetInstance();
	chunkMang->LoadChunk(GameSmith::ID(2037171962, 44136, 18147, 1874979930048970142));
}

void TestingLayer::OnImGuiRender()
{
}

void TestingLayer::OnUpdate()
{
	auto renderManager = GameSmith::RenderingManager::GetInstance();
	renderManager->SetForClear(m_RenderTex);
}
