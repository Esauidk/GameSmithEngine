#pragma once
#include "Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Mesh/Mesh.h"

namespace GameSmith {
	class GE_API MeshAsset : public Asset
	{
	public:
		MeshAsset(std::string name) : Asset(name) {};
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		inline Ref<Mesh> GetMesh() { return m_Mesh; }

		SERIAL_FILE(Mesh, obj) // Temporarily take obj only asset
	private:
		Ref<Mesh> m_Mesh;
	};
};


