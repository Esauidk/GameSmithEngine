#include "gepch.h"
#include "MeshAsset.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/BasicStructs.h"
#include "GameSmithEngine/ThirdPartySetups/TinyObj/TinyObjLoaderWrapper.h"

namespace GameSmith {
	Ref<char> MeshAsset::Serialize()
	{
		// TODO: Implement
		return Ref<char>();
	}

	void MeshAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		// TODO: Implement
	}

	unsigned int MeshAsset::RequireSpace() const
	{
		// TODO: Implement
		return 0;
	}

	void MeshAsset::Deserialize(char* inData, unsigned int size)
	{
		tinyobj::ObjReader reader;
		GE_CORE_ASSERT(reader.ParseFromString(inData, ""), "Could not successfully parse .obj mesh file");

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();

		GE_CORE_ASSERT(shapes[0].mesh.num_face_vertices[0] == 3, "This model does not use triangles");

		std::vector<VertexStruct> verticies;
		std::vector<unsigned int> indicies;
		std::unordered_set<unsigned int> viewedIndicies;

		for (size_t i = 0; i < attrib.vertices.size() / 3; i++) {
			VertexStruct vertex;

			vertex.pos = {
				attrib.vertices[3 * i],
				attrib.vertices[(3 * i) + 1],
				attrib.vertices[(3 * i) + 2]
			};
			verticies.push_back(vertex);
		}

		auto rManager = RenderingManager::GetInstance();
		GE_CORE_ASSERT(rManager != nullptr, "Cannot load mesh without render manager");

		for (size_t s = 0; s < shapes.size(); s++) {
			indicies.clear();
			for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[i];

				bool newVert = (viewedIndicies.find(idx.vertex_index) == viewedIndicies.end());

				if (newVert) {
					VertexStruct& v = verticies[idx.vertex_index];

					if (attrib.normals.size() > 0) {
						v.normal = {
							attrib.normals[3 * idx.normal_index],
							attrib.normals[(3 * idx.normal_index) + 1],
							attrib.normals[(3 * idx.normal_index) + 2]
						};
					}
					else {
						v.normal = { 0, 0, 0 };
					}

					if (attrib.texcoords.size() > 0) {
						v.uv = {
							attrib.texcoords[2 * idx.texcoord_index],
							attrib.texcoords[(2 * idx.texcoord_index) + 1]
						};
					}
					else {
						v.uv = { 0, 0 };
					}

					viewedIndicies.insert(idx.vertex_index);
				}


				indicies.push_back(idx.vertex_index);
			}

			auto index = rManager->GetRenderAPI()->CreateIndexBuffer(
				indicies.data(),
				(unsigned int)indicies.size()
			);

			m_SubMeshes.emplace_back(shapes[s].name, index);
		}

		m_Vert = rManager->GetRenderAPI()->CreateVertexBuffer(
			(BYTE*)verticies.data(),
			sizeof(VertexStruct),
			(int)verticies.size()
		);
	}

};