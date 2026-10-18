#include "gepch.h"
#include "ObjMeshSerializer.h"
#include "GameSmithEngine/ThirdPartySetups/TinyObj/TinyObjLoaderWrapper.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"


namespace GameSmith {
	Ref<char> GameSmith::ObjMeshSerializer::SerializeMesh(Ref<Mesh> mesh, unsigned int* outSize)
	{
		unsigned int size = 0;
		std::vector<std::string> vertexStrings;
		std::vector<std::string> uvStrings;
		std::vector<std::string> normalStrings;
		for (const auto& vertex : mesh->GetVertices()) {
			std::string vertexString = std::format("v {:.6f} {:.6f} {:.6f}\n", vertex.pos.x, vertex.pos.y, vertex.pos.z);
			std::string uvString = std::format("vt {:.6f} {:.6f}\n", vertex.uv.x, vertex.uv.y);
			std::string normalString = std::format("vn {:.6f} {:.6f} {:.6f}\n", vertex.normal.x, vertex.normal.y, vertex.normal.z);

			size += vertexString.length();
			size += uvString.length();
			size += normalString.length();

			vertexStrings.push_back(vertexString);
			uvStrings.push_back(uvString);
			normalStrings.push_back(normalString);
		}

		std::vector<std::string> faceStrings;
		for (const auto& subMesh : mesh->GetSubMeshes()) {
			unsigned int numIndices = subMesh.m_Indicies.size();
			for (unsigned int i = 0; i < numIndices; i += 3) {
				std::string faceString = std::format(
					"f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}\n", 
					subMesh.m_Indicies[i] + 1, subMesh.m_Indicies[i + 1] + 1, subMesh.m_Indicies[i + 2] + 1
				);
				size += faceString.length();
				faceStrings.push_back(faceString);
			}
		}

		Ref<char> buffer(new char[size]);
		BinaryStreamWriter writer(buffer.get(), size);
		for (const auto& vertexString : vertexStrings) {
			writer.WriteString(vertexString);
		}

		for (const auto& uvString : uvStrings) {
			writer.WriteString(uvString);
		}

		for (const auto& normalString : normalStrings) {
			writer.WriteString(normalString);
		}

		for (const auto& faceString : faceStrings) {
			writer.WriteString(faceString);
		}

		*outSize = size;
		return buffer;
	}

	Ref<Mesh> ObjMeshSerializer::DeserializeMesh(const char* data, unsigned int size)
	{
		tinyobj::ObjReader reader;
		GE_CORE_ASSERT(reader.ParseFromString(data, ""), "Could not successfully parse .obj mesh file");

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();

		GE_CORE_ASSERT(shapes[0].mesh.num_face_vertices[0] == 3, "This model does not use triangles");

		std::vector<VertexData> verticies;
		std::vector<SubMesh> subMeshes;
		std::unordered_set<unsigned int> viewedIndicies;

		for (size_t i = 0; i < attrib.vertices.size() / 3; i++) {
			VertexData vertex;

			vertex.pos = {
				attrib.vertices[3 * i],
				attrib.vertices[(3 * i) + 1],
				attrib.vertices[(3 * i) + 2]
			};
			verticies.push_back(vertex);
		}

		for (size_t s = 0; s < shapes.size(); s++) {
			std::vector<unsigned int> indicies;

			for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[i];

				bool newVert = (viewedIndicies.find(idx.vertex_index) == viewedIndicies.end());
				if (newVert) {
					VertexData& v = verticies[idx.vertex_index];

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

			subMeshes.emplace_back(shapes[s].name, indicies);
		}

		return Ref<Mesh>(new Mesh(verticies, subMeshes));
	}
};

