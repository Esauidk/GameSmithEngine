#include "gepch.h"
#include "Components/ComponentFactory.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Transform.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	const std::string GameObject::DEFAULT_GAMEOBJECT_NAME = "GameObject";

	GameObject::GameObject(std::string name) : m_Name(name), m_Transform(new Transform()) {}

	void GameObject::SetName(std::string newName)
	{
		auto con = GameObjectManager::GetInstance()->FindGameObject(m_Name);
		GameObjectProxy::UpdateManagerStoredNames(newName, con);
		m_Name = newName;
	}

	Connection<Component> GameObject::AddComponent(std::string componentName)
	{
		auto comp = ComponentFactory::GenerateComponent(componentName, this, m_Transform.get());

		auto updater = GameplayUpdater::GetInstance();

		if (updater != nullptr) {
			updater->Register(comp);
		}
		else {
			GE_CORE_ERROR("Unable to get reference to gameplay updater");
		}

		m_Components.emplace_back(comp);

		return comp;
	}

	Ref<char> GameObject::Serialize()
	{
		BinaryStreamWriter writer(RequiredSpace());
		
		GameObjectSerialMetadata md;
		md.numComponents = (unsigned int)m_Components.size();
		md.IDdata = GetID().getData();

		writer.WriteClass<GameObjectSerialMetadata>(&md);

		m_Transform->Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());
		writer.MoveCurPtr(m_Transform->RequiredSpace());

		writer.WriteString(GetName());

		for (auto comp : m_Components) {
			writer.WriteString(comp->GetName());
			idData data = comp->GetID().getData();
			writer.WriteClass<idData>(&data);
			comp->SerializeRegistry(writer.GetCurPtr(), writer.GetRemainingSpace());
		}

		return writer.GetBuffer();
	}

	void GameObject::Serialize(char* byteStream, unsigned int availableBytes)
	{
		unsigned int size = RequiredSpace();
		GE_CORE_ASSERT(availableBytes >= size, "Not enough space to serialize gameobject");

		BinaryStreamWriter writer(byteStream, size);
		
		GameObjectSerialMetadata md;
		md.numComponents = (unsigned int)m_Components.size();
		md.IDdata = GetID().getData();

		writer.WriteClass<GameObjectSerialMetadata>(&md);

		m_Transform->Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());
		writer.MoveCurPtr(m_Transform->RequiredSpace());

		writer.WriteString(GetName());

		for (auto comp : m_Components) {
			writer.WriteString(comp->GetName());
			idData data = comp->GetID().getData();
			writer.WriteClass<idData>(&data);
			comp->SerializeRegistry(writer.GetCurPtr(), writer.GetRemainingSpace());
		}

	}

	unsigned int GameObject::RequiredSpace() const
	{
		unsigned int size = sizeof(GameObjectSerialMetadata);

		size += m_Transform->RequiredSpace();

		size += (unsigned int)GetName().length() + 1;

		for (auto comp : m_Components) {
			size += (unsigned int)comp->GetName().length() + 1;
			size += sizeof(idData);
			size += comp->RegistrySerializationSize();
		}

		return size;
	}

	void GameObject::Deserialize(char* inData, unsigned int size)
	{
		m_Components.clear();

		BinaryStreamReader reader(inData, size);

		auto md = reader.ReadClass<GameObjectSerialMetadata>();
		ID newID(md->IDdata);
		auto con = GameObjectManager::GetInstance()->FindGameObject(m_Name);
		GameObjectProxy::UpdateManagerStoredID(newID, con);
		SetID(newID);

		m_Transform->Deserialize(reader.GetCurPtr(), reader.GetRemainingBytes());
		reader.MoveForward(m_Transform->RequiredSpace());

		SetName(reader.GetString());

		for (unsigned int i = 0; i < md->numComponents; i++) {
			auto compName = reader.GetString();
			auto id = reader.ReadClass<idData>();
			ID newID(*id);
			auto comp = AddComponent(compName).lock();
			comp->DeserializeRegistry(reader.GetCurPtr(), reader.GetRemainingBytes());
			comp->SetID(newID);
			reader.MoveForward(comp->RegistrySerializationSize());
		}
	}
};