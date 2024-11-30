#pragma once
#include "ParameterContainer.h"
#include "GameSmithEngine/ResourceAssets/Asset.h"
#include "GameSmithEngine/Utilities/GUIDGenerator.h"
#include "UtilMacros.h"

namespace GameSmith {
	enum ExposedVariableFlags {
		AssetGameObject = 0x1
	};

	template<typename T>
	class ObjectRefContainer {
	public:
		ObjectRefContainer(std::function<bool(T)> typeChecker, std::string typeName, unsigned int flags) : m_TypeCheckFunc(typeChecker), m_TypeName(typeName), m_Flag(flags) {}
		bool TypeCheck(T toTest) const { return m_TypeCheckFunc(toTest); }
		void AssignRef(T toAssign) {
			if (!TypeCheck(toAssign)) {
				return;
			}

			m_CopyRef = toAssign;
		}
		void AssignID(ID id) { m_ExpectedID = id; }
		std::string GetTypeName() const { return m_TypeName; }
		ID GetCurrentRefID() const { return m_ExpectedID; }
		T GetCurrentRef() const { return m_CopyRef; }
		unsigned int GetFlags() const { return m_Flag; }
	private:
		std::function<bool(T)> m_TypeCheckFunc;
		std::string m_TypeName;
		T m_CopyRef;
		ID m_ExpectedID;
		unsigned int m_Flag;
	};

	// A ref container that only wants to establish a watching connection to an object
	using ConnectionContainer = ObjectRefContainer<Connection<IDObject>>;
	// A ref container that wants to both establish and own a connection to an asset/serializable object
	using AssetRefContainer = ObjectRefContainer<Ref<Asset>>;

	class GE_API ExposedVariableRegistry : public Serializeable
	{
	public:
		/* Adds a definition for an exposed variable
		* variableName: A string to tie to this new exposed variable definition
		* entry: a pointer to update with new variable data
		* entryType: the type of variable to treat this entry as
		*/
		inline void AddExposedVariable(std::string variableName, void* entry, ContainerDataType entryType) { if (entry == nullptr) return; m_ValueRegistry.insert({ variableName, {entry, entryType} }); }

		/* Adds a definition for an exposed connection
		* refName: A string to tie this new exposed connection definition
		* entry: a pointer to update with new connection data
		* typeName: a string to represent the underlying object being referenced (normally used for debugging or informational purposes)
		* flags: Any special flags to determine how the connection is interpreted (view ExposedVariableFlags)
		*/
		template<typename T>
		inline void AddExposedConnection(std::string refName, Connection<IDObject>* entry, std::string typeName, unsigned int flags = 0) { 
			if (entry == nullptr) return;

			m_ConnectionsRegistry.insert(
				{ 
					refName, 
					{
						entry, 
						[](Connection<IDObject>* origin, Connection<IDObject> curPtr) 
						{
							*((Connection<T>*)origin) = CastPtr<T>(curPtr.lock());
						},

						[](Connection<IDObject> curPtr)
						{
							return CastPtr<T>(curPtr.lock()) != nullptr;
						},
						typeName,
						{},
						flags
					} 
				}
			);
		}

		/* Adds a definition for an exposed asset
		* refName: A string to tie this new exposed reference definition
		* entry: a pointer to update with new reference data
		* typeName: a string to represent the underlying object being referenced (normally used for debugging or informational purposes)
		* flags: Any special flags to determine how the reference is interpreted (view ExposedVariableFlags)
		*/
		template<typename T>
		inline void AddExposedAsset(std::string refName, Ref<Asset>* entry, std::string typeName, unsigned int flags = 0) {
			if (entry == nullptr) return;

			m_AssetRegistry.insert(
				{
					refName,
					{
						entry,
						[](Ref<Asset>* origin, Ref<Asset> curPtr)
						{
							*((Ref<T>*)origin) = CastPtr<T>(curPtr);
						},

						[](Ref<Asset> curPtr)
						{
							return CastPtr<T>(curPtr) != nullptr;
						},
						typeName,
						{},
						flags
					}
				}
			);
		}

		void GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap);
		void GenerateConnectionsMap(std::unordered_map<std::string, Ref<ConnectionContainer>>* outMap);
		void GenerateAssetMap(std::unordered_map<std::string, Ref<AssetRefContainer>>* outMap);

		void BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap);
		void BootstrapFromConnectionsMap(const std::unordered_map<std::string, Ref<ConnectionContainer>>& inMap);
		void BootstrapFromAssetMap(const std::unordered_map<std::string, Ref<AssetRefContainer>>& inMap);


		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		struct RegistrySerializeMetadata {
			unsigned int numVariables;
			unsigned int numRefs;
		};

		struct ExposedVariableEntry {
			void* originalVariableRef;
			ContainerDataType variableDataType;
			unsigned int flags;
		};

		template<typename T>
		struct ExposedObjectRefEntry {
			T* originalRefRef;
			std::function<void(T*, T)> assignmentFunction;
			std::function<bool(T)> conversionFunction;
			std::string typeName;
			ID objectID;
			unsigned int flag;
		};

		using ExposedAssetEntry = ExposedObjectRefEntry<Ref<Asset>>;
		using ExposedConnectionEntry = ExposedObjectRefEntry<Connection<IDObject>>;

	private:
		std::unordered_map<std::string, ExposedVariableEntry> m_ValueRegistry;
		std::unordered_map<std::string, ExposedConnectionEntry> m_ConnectionsRegistry;
		std::unordered_map<std::string, ExposedAssetEntry> m_AssetRegistry;
	};
};


