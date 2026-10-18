#pragma once
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Mesh/Mesh.h"
#include "MeshSerializers/MeshSerializer.h"

namespace GameSmith {
	class GE_API MeshAsset : public Asset
	{
	public:
		MeshAsset(std::string name, Scope<MeshSerializer> serializer) : Asset(name), m_Serializer(std::move(serializer)) {};
		MeshAsset(std::string name, Ref<Mesh> mesh, Scope<MeshSerializer> serializer) : Asset(name), m_Mesh(mesh), m_Serializer(std::move(serializer)) {};
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		inline Ref<Mesh> GetMesh() { return m_Mesh; }
	private:
		Ref<Mesh> m_Mesh;
		Scope<MeshSerializer> m_Serializer;
	};
};


