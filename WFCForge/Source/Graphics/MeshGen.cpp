#include "Core/Core.hpp"
#include "Graphics/MeshGen.hpp"


#define VEC3_ADD(out, a, b) \
		{ \
			out.x = a.x + b.x; \
			out.y = a.y + b.y; \
			out.y = a.z + b.z; \
		}

#define VEC3_SUB(out, a, b) \
		{ \
			out.x = a.x - b.x; \
			out.y = a.y - b.y; \
			out.y = a.z - b.z; \
		}

#define VEC3_DOT(out, a, b) \
		out = a.x * b.x + a.y * b.y + a.z * b.z; 

#define VEC3_CROSS(out, a, b) \
		{ \
			out.x = a.y * b.z - b.y * a.z; \
			out.y = a.x * b.z - b.x * a.z; \
			out.z = a.x * b.y - b.x * a.y; \
		} 

#define VEC3_LENGTH(a) \
		sqrt(a.x * a.x + a.y * a.y + a.z * a.z);

#define VEC3_NORMALIZE(a) \
		{ \
			float inverseLength##__LINE__ = 1.0f / VEC3_LENGTH(a); \
			a.x = a.x * inverseLength##__LINE__; \
			a.y = a.y * inverseLength##__LINE__; \
			a.z = a.z * inverseLength##__LINE__; \
		}

namespace WFCForge
{

	static void RecalculateNormals(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount)
	{
		glm::vec3 e1(0.0f);
		glm::vec3 e2(0.0f);
		glm::vec3 no(0.0f);
	
		for (int i = 0 ; i < vertexCount ; i++)
		{
			vertices[i].normal = glm::vec4(0.0f);
		}

		for (int i = 0 ; i < indexCount ; i += 3)
		{
			glm::vec4& tmp4a = vertices[indices[i]].position;
			glm::vec4& tmp4b = vertices[indices[i+1]].position;
			glm::vec4& tmp4c = vertices[indices[i+2]].position;
			VEC3_SUB(e1, tmp4a, tmp4b);
			VEC3_SUB(e2, tmp4c, tmp4b);
			VEC3_CROSS(no, e1, e2);
			VEC3_ADD(vertices[indices[i]].normal, no, vertices[indices[i]].normal);
			VEC3_ADD(vertices[indices[i+1]].normal, no, vertices[indices[i+1]].normal);
			VEC3_ADD(vertices[indices[i+2]].normal, no, vertices[indices[i+2]].normal);
		}

		for (uint64_t i = 0 ; i < vertexCount ; i++)
		{
			VEC3_NORMALIZE(vertices[i].normal);
			vertices[i].normal.w = 0.0f;
		}
	}



	void MeshGen::FloorTile(Mesh* mesh)
	{
		WFC_ASSERT(mesh, "Mesh is null");
		if (mesh->IsSetup())
			mesh->Destroy();
		static Vertex vertices[4] =
		{
			Vertex(glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		};
		static uint32_t indices[6] =
		{
			0, 1, 3,
			1, 2, 3
		};
		mesh->Setup(WFC_ARRAY_SIZE(vertices), vertices, WFC_ARRAY_SIZE(indices), indices);
	}

	void MeshGen::Cube(Mesh* mesh)
	{
		WFC_ASSERT(mesh, "Mesh is null");
		if (mesh->IsSetup())
			mesh->Destroy();
		static Vertex vertices[] =
		{
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		};
		static uint32_t indices[] =
		{
			0, 1, 2,
			3, 4, 5,
			6, 7,  8,
			9, 10, 11,
			12, 13, 14,
			15, 16, 17,
			18, 19, 20,
			21, 22, 23,
			24, 25, 26,
			27, 28, 29,
			30, 31, 32,
			33, 34, 35
		};
		RecalculateNormals(vertices, WFC_ARRAY_SIZE(vertices), indices, WFC_ARRAY_SIZE(indices));
		mesh->Setup(WFC_ARRAY_SIZE(vertices), vertices, WFC_ARRAY_SIZE(indices), indices);
	}
}