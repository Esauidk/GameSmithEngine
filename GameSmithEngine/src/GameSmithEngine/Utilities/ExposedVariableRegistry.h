#pragma once
#include "ParameterContainer.h"
#include "GameSmithEngine/ResourceAssets/Serializable.h"
#include "GameSmithEngine/Utilities/GUIDGenerator.h"

namespace GameSmith {
	enum ExposedVariableFlags {
		Asset = 0x1
	};

	class GE_API RefContainer {
	public:
		RefContainer(std::function<bool(Connection<IDObject>)> typeChecker, std::string typeName) : m_TypeCheckFunc(typeChecker), m_TypeName(typeName) {}
		bool TypeCheck(Connection<IDObject> toTest) const { return m_TypeCheckFunc(toTest); }
		void AssignRef(Connection<IDObject> toAssign, unsigned int flags);
		void AssignID(ID id) { m_ExpectedID = id; }
		std::string GetTypeName() const { return m_TypeName; }
		ID GetCurrentRefID() const { return m_ExpectedID; }
		Connection<IDObject> GetCurrentRef() const { return m_CopyRef; }
		unsigned int GetFlags() { return m_Flag; }
	private:
		std::function<bool(Connection<IDObject>)> m_TypeCheckFunc;
		std::string m_TypeName;
		Connection<IDObject> m_CopyRef;
		ID m_ExpectedID;
		unsigned int m_Flag;
	};

	class GE_API ExposedVariableRegistry : public Serializeable
	{
	public:
		inline void AddExposedVariable(std::string variableName, void* entry, ContainerDataType entryType) { if (entry == nullptr) return; m_ValueRegistry.insert({ variableName, {entry, entryType} }); }

		template<typename T>
		inline void AddExposedRef(std::string refName, Connection<IDObject>* entry, std::string typeName) { 
			if (entry == nullptr) return;

			m_RefRegistry.insert(
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
						typeName
					} 
				}
			);
		}

		void GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap);
		void GenerateReferenceMap(std::unordered_map<std::string, Ref<RefContainer>>* outMap);

		void BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap);
		void BootstrapFromRefMap(const std::unordered_map<std::string, Ref<RefContainer>>& inMap);


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

		struct ExposedRefEntry {
			Connection<IDObject>* originalRefRef;
			std::function<void(Connection<IDObject>*, Connection<IDObject>)> assignmentFunction;
			std::function<bool(Connection<IDObject>)> conversionFunction;
			std::string typeName;
			ID objectID;
			unsigned int flag;
		};

	private:
		std::unordered_map<std::string, ExposedVariableEntry> m_ValueRegistry;
		std::unordered_map<std::string, ExposedRefEntry> m_RefRegistry;
	};
};


