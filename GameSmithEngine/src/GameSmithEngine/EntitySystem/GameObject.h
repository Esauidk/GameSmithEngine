#pragma once
#include "gepch.h"
#include "Components/Component.h"
#include "GameplayUpdater.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/EntitySystem/Transform.h"

#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"


namespace GameSmith {

	class GE_API GameObject : public Asset {
	public:
		GameObject(std::string name = DEFAULT_GAMEOBJECT_NAME);
		inline std::string GetName() const { return m_Name; }
		void SetName(std::string newName);
		inline Connection<Transform> GetTransform() { return m_Transform; }

		Connection<Component> AddComponent(std::string componentName);

		template<typename T>
		Connection<T> AddComponent() {
			auto component = Ref<T>(new T(this, m_Transform.get()));
			
			auto updater = GameplayUpdater::GetInstance();

			if (updater != nullptr) {
				updater->Register(component);
			}
			else {
				GE_CORE_ERROR("Unable to get reference to gameplay updater");
			}

			m_Components.emplace_back(component);

			return component;
		}

		template<typename T>
		Connection<T> GetComponent() {
			for (Ref<Component> comp : m_Components) {
				auto cast = CastPtr<T>(comp);
				if (cast != nullptr) {
					return cast;
				}
			}

			return Ref<T>(nullptr);
		}

		template<typename T>
		void RemoveComponent(Connection<T> component) {
			auto lockComp = component.lock();
			if (lockComp->GetGameObject() == this) {
				for (auto it = m_Components.begin(); it < m_Components.end(); it++) {
					if ((*it).get() == lockComp.get()) {
						lockComp->OnDestroy();
						m_Components.erase(it);
						break;
					}
				}
			}
		}

		template<typename T>
		void GetComponents(std::vector<Connection<T>>* outContainer) {

			for (Ref<Component> comp : m_Components) {
				auto cast = CastPtr<T>(comp);
				if (cast != nullptr) {
					outContainer->push_back(cast);
				}
			}
		}

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;


	public:
		const static std::string DEFAULT_GAMEOBJECT_NAME;
	private:
		std::string m_Name;
		Ref<Transform> m_Transform;
		std::vector<Ref<Component>> m_Components;

		struct GameObjectSerialMetadata {
			unsigned int numComponents;
			idData IDdata;
		};
	};
};
