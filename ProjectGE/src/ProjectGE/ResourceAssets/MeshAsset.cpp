#include "gepch.h"
#include "MeshAsset.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/RenderingManager.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

namespace ProjectGE {
	void MeshAsset::Init()
	{
		char* data = GetResourceData();
		UINT size = GetResourceSize();

		GE_CORE_ASSERT(size >= sizeof(MeshMetadata), "Not enough space to hold Mesh metadata");
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

		unsigned int* indicies = (unsigned int*)data;

		auto rManager = RenderingManager::GetInstance();
		GE_CORE_ASSERT(rManager != nullptr, "Cannot load mesh without render manager");

		m_Vert = rManager->GetRenderAPI()->CreateVertexBuffer(
			(BYTE*)vertexData, 
			sizeof(VertexStruct), 
			metadata->numVerticies
		);

		BufferLayoutBuilder layout = { {"POSITION", ProjectGE::ShaderDataType::Float3}, {"UV_TEXCOORD", ProjectGE::ShaderDataType::Float2} };

		m_Vert->AttachLayout(layout);

		m_Index = rManager->GetRenderAPI()->CreateIndexBuffer(
			indicies,
			metadata->numFaces * 3
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