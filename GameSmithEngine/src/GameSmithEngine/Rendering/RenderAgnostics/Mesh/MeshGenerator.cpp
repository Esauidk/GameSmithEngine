#include "gepch.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Mesh/MeshGenerator.h"
#include <glm/glm.hpp>

#define CUBE_VERTICES 8
namespace GameSmith {
	struct TriangleIndex {
		unsigned int v1;
		unsigned int v2;
		unsigned int v3;

		void AddToVector(std::vector<unsigned int>& indicies) const {
			indicies.push_back(v1);
			indicies.push_back(v2);
			indicies.push_back(v3);
		}

		glm::vec3 GetNormal(const std::vector<VertexData>& vertices) const {
			glm::vec3 p1 = vertices[v1].pos;
			glm::vec3 p2 = vertices[v2].pos;
			glm::vec3 p3 = vertices[v3].pos;
			glm::vec3 edge1 = p2 - p1;
			glm::vec3 edge2 = p3 - p1;
			return glm::normalize(glm::cross(edge1, edge2));
		}
	};

	static void RemoveSharedVerticies(std::vector<TriangleIndex>& triangles, std::vector<VertexData>& verticies) {
		std::vector<unsigned int> vertexCounts(verticies.size(), 0);

		for (const auto& triangle : triangles) {
			vertexCounts[triangle.v1]++;
			vertexCounts[triangle.v2]++;
			vertexCounts[triangle.v3]++;
		}

		for (auto& triangle : triangles) {
			if (vertexCounts[triangle.v1] > 1) {
				unsigned int newIndex = verticies.size();
				verticies.push_back(verticies[triangle.v1]);
				vertexCounts[triangle.v1]--;
				triangle.v1 = newIndex;
			}

			if (vertexCounts[triangle.v2] > 1) {
				unsigned int newIndex = verticies.size();
				verticies.push_back(verticies[triangle.v2]);
				vertexCounts[triangle.v2]--;
				triangle.v2 = newIndex;
			}

			if (vertexCounts[triangle.v3] > 1) {
				unsigned int newIndex = verticies.size();
				verticies.push_back(verticies[triangle.v3]);
				vertexCounts[triangle.v3]--;
				triangle.v3 = newIndex;
			}
		}
	};

	Ref<Mesh> CreateCube(unsigned int size)
	{

		std::vector<VertexData> verticies(CUBE_VERTICES);
		std::vector<TriangleIndex> triangles;

		const float centerDistance = size / 2.0f;

		// Position Calculations
		verticies[0].pos = glm::vec3(-centerDistance, -centerDistance, -centerDistance);
		verticies[1].pos = glm::vec3(centerDistance, -centerDistance, -centerDistance);
		verticies[2].pos = glm::vec3(centerDistance, centerDistance, -centerDistance);
		verticies[3].pos = glm::vec3(-centerDistance, centerDistance, -centerDistance);
		verticies[4].pos = glm::vec3(-centerDistance, -centerDistance, centerDistance);
		verticies[5].pos = glm::vec3(centerDistance, -centerDistance, centerDistance);
		verticies[6].pos = glm::vec3(centerDistance, centerDistance, centerDistance);
		verticies[7].pos = glm::vec3(-centerDistance, centerDistance, centerDistance);

		// Triangle Definitions
		// 
		// Face 1
		// Triangle 1
		TriangleIndex t1 = { 0, 2, 1 };
		triangles.push_back(t1);
		// Triangle 2
		TriangleIndex t2 = { 3, 2, 0 };
		triangles.push_back(t2);

		// Face 2
		// Triangle 3
		TriangleIndex t3 = { 4, 3, 0 };
		triangles.push_back(t3);
		// Triangle 4
		TriangleIndex t4 = { 7, 3, 4 };
		triangles.push_back(t4);

		// Face 3
		// Triangle 5
		TriangleIndex t5 = { 5, 7, 4 };
		triangles.push_back(t5);
		// Triangle 6
		TriangleIndex t6 = { 6, 7, 5 };
		triangles.push_back(t6);

		// Face 4
		// Triangle 7
		TriangleIndex t7 = { 5, 2, 1 };
		triangles.push_back(t7);
		//Triangle 8
		TriangleIndex t8 = { 6, 2, 5 };
		triangles.push_back(t8);

		// Face 5
		// Triangle 9
		TriangleIndex t9 = { 2, 7, 3 };
		triangles.push_back(t9);
		// Triangle 10
		TriangleIndex t10 = { 6, 7, 2 };
		triangles.push_back(t10);

		// Face 6
		// Triangle 11
		TriangleIndex t11 = { 1, 4, 0 };
		triangles.push_back(t11);
		// Trangle 12
		TriangleIndex t12 = { 5, 4, 1 };
		triangles.push_back(t12);

		RemoveSharedVerticies(triangles, verticies);

		// UVs
		for (size_t i = 0; i < triangles.size(); i+=2) {
			verticies[triangles[i].v1].uv = glm::vec2(0.0f, 0.0f);
			verticies[triangles[i].v2].uv = glm::vec2(1.0f, 1.0f);
			verticies[triangles[i].v3].uv = glm::vec2(1.0f, 0.0f);
			verticies[triangles[i+1].v1].uv = glm::vec2(0.0f, 1.0f);
			verticies[triangles[i+1].v2].uv = glm::vec2(1.0f, 1.0f);
			verticies[triangles[i+1].v3].uv = glm::vec2(0.0f, 0.0f);
		}
		
		std::vector<unsigned int> indicies;
		for (const auto& triangle : triangles) {
			glm::vec3 normal = triangle.GetNormal(verticies);
			verticies[triangle.v1].normal = normal;
			verticies[triangle.v2].normal = normal;
			verticies[triangle.v3].normal = normal;
			triangle.AddToVector(indicies);
		}

		SubMesh subMesh;
		subMesh.m_SubMeshName = "Cube";
		subMesh.m_Indicies = indicies;
		
		return Ref<Mesh>(new Mesh(verticies, {subMesh}));
	}
	Ref<Mesh> CreateSphere()
	{
		return Ref<Mesh>();
	}
	Ref<Mesh> CreateCylinder()
	{
		return Ref<Mesh>();
	}
	Ref<Mesh> CreateCone()
	{
		return Ref<Mesh>();
	}
	Ref<Mesh> CreateTorus()
	{
		return Ref<Mesh>();
	}
	Ref<Mesh> CreatePlane()
	{
		return Ref<Mesh>();
	}
	Ref<Mesh> CreateCapsule()
	{
		return Ref<Mesh>();
	}
};