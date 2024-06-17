#include "gtest/gtest.h"

#include "GameSmithEngine/Utilities/ExposedVariableRegistry.h"

TEST(ExposedVariableRegistryTest, GenerateVariableMapNoVariable) {
	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> testMap;
	GameSmith::ExposedVariableRegistry registry;
	
	EXPECT_EQ(testMap.size(), 0);
	registry.GenerateVariableMap(&testMap);
	EXPECT_EQ(testMap.size(), 0);
}

TEST(ExposedVariableRegistryTest, GenerateVariableMapSingleEntry) {
	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> testMap;
	GameSmith::ExposedVariableRegistry registry;

	EXPECT_EQ(testMap.size(), 0);
	bool testVar;
	GameSmith::ExposedVariableEntry entry = { &testVar, GameSmith::ContainerDataType::Int };
	EXPECT_NO_THROW(registry.AddExposedVariable("TestVariable", entry));
	
	registry.GenerateVariableMap(&testMap);
	EXPECT_EQ(testMap.size(), 1);
	EXPECT_TRUE(testMap.contains("TestVariable"));

	const auto& mapEntry = testMap.find("TestVariable");
	EXPECT_EQ(mapEntry->second->GetName(), "TestVariable");
	EXPECT_EQ(mapEntry->second->GetType(), GameSmith::ContainerDataType::Int);
}

TEST(ExposedVariableRegistryTest, GenerateVariableMapMultipleEntry) {
	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> testMap;
	GameSmith::ExposedVariableRegistry registry;

	EXPECT_EQ(testMap.size(), 0);

	bool testVar1;
	GameSmith::ExposedVariableEntry entry1 = { &testVar1, GameSmith::ContainerDataType::Int };
	EXPECT_NO_THROW(registry.AddExposedVariable("TestVariable1", entry1));

	int testVar2;
	GameSmith::ExposedVariableEntry entry2 = { &testVar2, GameSmith::ContainerDataType::Int };
	EXPECT_NO_THROW(registry.AddExposedVariable("TestVariable2", entry2));

	registry.GenerateVariableMap(&testMap);
	EXPECT_EQ(testMap.size(), 2);
	EXPECT_TRUE(testMap.contains("TestVariable1"));
	EXPECT_TRUE(testMap.contains("TestVariable2"));


	const auto& mapEntry1 = testMap.find("TestVariable1");
	EXPECT_EQ(mapEntry1->second->GetName(), "TestVariable1");
	EXPECT_EQ(mapEntry1->second->GetType(), GameSmith::ContainerDataType::Int);

	const auto& mapEntry2 = testMap.find("TestVariable2");
	EXPECT_EQ(mapEntry2->second->GetName(), "TestVariable2");
	EXPECT_EQ(mapEntry2->second->GetType(), GameSmith::ContainerDataType::Int);
}

TEST(ExposedVariableRegistryTest, AddExposedVariableNullReference) {
	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> testMap;
	GameSmith::ExposedVariableRegistry registry;

	EXPECT_EQ(testMap.size(), 0);
	GameSmith::ExposedVariableEntry entry = { nullptr, GameSmith::ContainerDataType::Bool };
	EXPECT_NO_THROW(registry.AddExposedVariable("TestVariable", entry));

	registry.GenerateVariableMap(&testMap);
	EXPECT_EQ(testMap.size(), 0);
}

TEST(ExposedVariableRegistryTest, BootstrapVariableMapSingleVariable) {
	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> testMap;
	GameSmith::ExposedVariableRegistry registry;

	EXPECT_EQ(testMap.size(), 0);
	int testVar;
	GameSmith::ExposedVariableEntry entry = { &testVar, GameSmith::ContainerDataType::Int };
	EXPECT_NO_THROW(registry.AddExposedVariable("TestVariable", entry));

	registry.GenerateVariableMap(&testMap);
	EXPECT_EQ(testMap.size(), 1);
	EXPECT_TRUE(testMap.contains("TestVariable"));

	const auto& mapEntry = testMap.find("TestVariable");
	GameSmith::CastPtr<GameSmith::IntContainer>(mapEntry->second)->SetData(5);

	registry.BootstrapFromValueMap(testMap);

	EXPECT_EQ(testVar, 5);
}

TEST(ExposedVariableRegistryTest, BootstrapVariableMapMultipleVariable) {
	std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> testMap;
	GameSmith::ExposedVariableRegistry registry;

	EXPECT_EQ(testMap.size(), 0);
	int testVar;
	GameSmith::ExposedVariableEntry entry = { &testVar, GameSmith::ContainerDataType::Int };
	EXPECT_NO_THROW(registry.AddExposedVariable("TestVariable", entry));

	int testVar1;
	GameSmith::ExposedVariableEntry entry1 = { &testVar1, GameSmith::ContainerDataType::Int };
	EXPECT_NO_THROW(registry.AddExposedVariable("TestVariable1", entry1));

	registry.GenerateVariableMap(&testMap);
	EXPECT_EQ(testMap.size(), 2);
	EXPECT_TRUE(testMap.contains("TestVariable"));
	EXPECT_TRUE(testMap.contains("TestVariable1"));

	const auto& mapEntry = testMap.find("TestVariable");
	GameSmith::CastPtr<GameSmith::IntContainer>(mapEntry->second)->SetData(5);

	const auto& mapEntry1 = testMap.find("TestVariable1");
	GameSmith::CastPtr<GameSmith::IntContainer>(mapEntry1->second)->SetData(4);

	registry.BootstrapFromValueMap(testMap);

	EXPECT_EQ(testVar, 5);
	EXPECT_EQ(testVar1, 4);
}