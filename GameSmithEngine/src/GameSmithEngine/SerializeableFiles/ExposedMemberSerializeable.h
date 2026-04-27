#pragma once
#include "Serializable.h"
#include "GameSmithEngine/Utilities/ExposedVariableRegistry.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	class GE_API ExposedMemberSerializeable : public AbstractBaseSerializeable, public virtual IExposedMembers {
	public:
		inline void GenerateVariableEntries(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap) override { m_Registry.GenerateVariableMap(outMap); }
		inline void GenerateConnectionEntries(std::unordered_map<std::string, Ref<ConnectionContainer>>* outMap) override { m_Registry.GenerateConnectionsMap(outMap); }
		inline void GenerateAssetEntries(std::unordered_map<std::string, Ref<AssetRefContainer>>* outMap) override { m_Registry.GenerateAssetMap(outMap); }
		inline const std::unordered_map<std::string, std::unordered_set<std::string>>& GetExposedGroupings() const override { return m_Registry.GetGroupingRegistry(); }

		inline void BootstrapVariableRegistry(std::unordered_map<std::string, Ref<ParameterContainer>>& variableEntries) override {
			m_Registry.BootstrapFromValueMap(variableEntries);
			PostRegistryBootstrap();
		}

		inline void BootstrapConnectionRegistry(std::unordered_map<std::string, Ref<ConnectionContainer>>& refEntries) override {
			m_Registry.BootstrapFromConnectionsMap(refEntries);
			PostRegistryBootstrap();
		}

		inline void BootstrapAssetRegistry(std::unordered_map<std::string, Ref<AssetRefContainer>>& refEntries) override {
			m_Registry.BootstrapFromAssetMap(refEntries);
			PostRegistryBootstrap();
		}

		// It's not recommended to rely on the default implementation of the serialization functions, as it adds a layer of indirection with the registry
		// that is not nessecary for most derive classes.
		// If possible, override this function and serialize the data connected to the registry directly
		Ref<char> Serialize() override {
			BinaryStreamWriter writer(RequiredSpace());
			Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());
			return writer.GetBuffer();
		};

		void Serialize(char* byteStream, unsigned int availableBytes) override { m_Registry.Serialize(byteStream, availableBytes); }

		// Returns the amount of bytes required to serialize the interface implementor
		unsigned int RequiredSpace() const override { return m_Registry.RequiredSpace(); }

		// Deserializes a byte array into the given class
		void Deserialize(char* inData, unsigned int size) override { 
			m_Registry.Deserialize(inData, size); 
			PostRegistryBootstrap();
		}
	protected:
		ExposedVariableRegistry m_Registry;
	};
}; 
