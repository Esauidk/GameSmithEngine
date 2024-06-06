#pragma once
#include "GameSmithEngine/ThirdPartySetups/GLM/GLMDefines.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameSmithEngine/ResourceAssets/Serializable.h"

namespace GameSmith {
	class Transform : public Serializeable
	{
	public:
		Transform();
		inline bool HasChanged() { return m_ModelMatrix != m_LastMatrix; }
		inline void SetPosition(glm::vec3 newPos) { m_Position = newPos; UpdateMatrix(); }
		inline glm::vec3 GetPosition() const { return m_Position; }
		inline void SetRotation(glm::vec3 newRotation) { m_Rotation = newRotation; UpdateMatrix(); }
		inline glm::vec3 GetRotation() const { return m_Rotation; }
		inline void SetScale(glm::vec3 newScale) { m_Scale = newScale; UpdateMatrix(); }
		inline glm::vec3 GetScale() const { return m_Scale; }

		inline void SetCoordinateFrame(Transform* newFrame) { m_CoordinateFrame = newFrame; newFrame->AddChildrenCoordinateFrame(this); UpdateMatrix(); }
		inline void AddChildrenCoordinateFrame(Transform* newFrame) { m_ChildrenFrames.push_back(newFrame); }
		void RemoveChildCoordinateFrame(Transform* oldFrame);

		inline glm::mat4 GetModelMatrix() const { return glm::transpose(m_ModelMatrix); }

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		void UpdateMatrix();
	private:
		std::vector<Transform*> m_ChildrenFrames;
		bool m_Changed;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_ModelMatrix;

		struct TransformSerializeData {
			glm::vec3 Position;
			glm::vec3 Rotation;
			glm::vec3 Scale;
		};

		// Has no ownership of this frame
		Transform* m_CoordinateFrame;

		glm::mat4 m_LastMatrix;
	};
};


