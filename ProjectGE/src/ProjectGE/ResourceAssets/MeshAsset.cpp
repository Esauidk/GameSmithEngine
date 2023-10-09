#include "gepch.h"
#include "MeshAsset.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/RenderingManager.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"
#include "ProjectGE/ThirdPartySetups/TinyObj/TinyObjLoaderWrapper.h"

namespace ProjectGE {
	void MeshAsset::Init()
	{
		char* data = GetResourceData();
		UINT size = GetResourceSize();

		tinyobj::ObjReader reader;
		GE_CORE_ASSERT(reader.ParseFromString(data, ""), "Could not successfully parse .obj mesh file");

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();

		GE_CORE_ASSERT(shapes[0].mesh.num_face_vertices[0] == 3, "This model does not use triangles");

		std::vector<VertexStruct> verticies;
		std::vector<unsigned int> indicies;

		for (size_t i = 0; i < attrib.vertices.size()/3; i++) {
			VertexStruct vertex;
			
			vertex.pos[0] = attrib.vertices[3 * i];
			vertex.pos[1] = attrib.vertices[(3 * i) + 1];
			vertex.pos[2] = attrib.vertices[(3 * i) + 2];
			verticies.push_back(vertex);

			/*if (attrib.normals.size() > 0) {
				vertex.normal[0] = attrib.normals[3 * i];
				vertex.normal[1] = attrib.normals[(3 * i) + 1];
				vertex.normal[2] = attrib.normals[(3 * i) + 2];
			}
			else {
				vertex.normal[0] = 0;
				vertex.normal[1] = 0;
				vertex.normal[2] = 0;
			}

			if (attrib.texcoords.size() > 0) {
				vertex.uv[0] = attrib.texcoords[2 * i];
				vertex.uv[1] = attrib.texcoords[(2 * i) + 1];
			}
			else {
				vertex.uv[0] = 0;
				vertex.uv[1] = 0;
			}*/
		}

		for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++) {
			tinyobj::index_t idx = shapes[0].mesh.indices[i];
			indicies.push_back(idx.vertex_index);
		}

		/*GE_CORE_ASSERT(size >= sizeof(MeshMetadata), "Not enough space to hold Mesh metadata");
		// Get metadata
		MeshMetadata* metadata = (MeshMetadata*)data;
		data += sizeof(MeshMetadata);

		bool fileFitsMeta = 
			size >=
			(
				sizeof(MeshMetadata)
				+ sizeof(VertexStruct) * metadata->numVerticies
				+ sizeof(unsigned short) * (metadata->numFaces * 3)
			);

		GE_CORE_ASSERT(fileFitsMeta, "File cannot fit data described in metadata, corruption?");

		// Make container for extracted vertex
		VertexStruct* vertexData = (VertexStruct*)data;
		data += sizeof(VertexStruct) * metadata->numVerticies;

		unsigned int* indicies = (unsigned int*)data;*/

		auto rManager = RenderingManager::GetInstance();
		GE_CORE_ASSERT(rManager != nullptr, "Cannot load mesh without render manager");

		m_Vert = rManager->GetRenderAPI()->CreateVertexBuffer(
			(BYTE*)verticies.data(), 
			sizeof(VertexStruct), 
			(int)verticies.size()
		);

		BufferLayoutBuilder layout = { {"POSITION", ProjectGE::ShaderDataType::Float3}, {"UV_TEXCOORD", ProjectGE::ShaderDataType::Float2} };

		m_Vert->AttachLayout(layout);

		m_Index = rManager->GetRenderAPI()->CreateIndexBuffer(
			indicies.data(),
			(unsigned int)indicies.size()
		);
		
	}

	void MeshAsset::Destroy()
	{
		m_Vert = nullptr;
		m_Index = nullptr;
	}

	void MeshAsset::SetGraphicsMesh()
	{
		auto manager = RenderingManager::GetInstance();

		if (manager != nullptr) {
			auto renderAPI = manager->GetRenderAPI();
			renderAPI->SetVertexBuffer(m_Vert);
			renderAPI->SetIndexBuffer(m_Index);
			renderAPI->DrawIndexed(m_Index->GetCount(), 1);
		}
		else {
			GE_CORE_INFO("RenderManager Not Initialized: Not Allocating GPU Resources for Texture Asset");
		}
	}
};