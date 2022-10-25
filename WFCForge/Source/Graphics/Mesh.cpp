#include "Core/Core.hpp"
#include "Graphics/Mesh.hpp"

#include <glad/glad.h>

namespace WFCForge
{



	void Mesh::Setup(uint32_t vertexCount, const Vertex* vertices, uint32_t indexCount, const uint32_t* indices)
	{
		WFC_ASSERT(!this->vao, "Mesh already setup");
		this->inc = indexCount;

		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertices, GL_STATIC_DRAW);
		glGenBuffers(1, &this->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCount, indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);
	}

	void Mesh::Destroy()
	{
		WFC_ASSERT(this->vao, "Mesh not yet setup");
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		this->vao = 0;
	}

	void Mesh::Render()
	{
		WFC_ASSERT(this->vao, "Mesh not yet setup");
		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, this->inc, GL_UNSIGNED_INT, 0);
	}

	void Mesh::RenderWireFrame()
	{
		WFC_ASSERT(this->vao, "Mesh not yet setup");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, this->inc, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Mesh::RenderInstanced(uint32_t count)
	{
		WFC_ASSERT(this->vao, "Mesh not yet setup");
		glBindVertexArray(this->vao);
		glDrawElementsInstanced(GL_TRIANGLES, this->inc, GL_UNSIGNED_INT, 0, count);
	}

	void Mesh::RenderWireFrameInstanced(uint32_t count)
	{
		WFC_ASSERT(this->vao, "Mesh not yet setup");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		glBindVertexArray(this->vao);
		glDrawElementsInstanced(GL_TRIANGLES, this->inc, GL_UNSIGNED_INT, 0, count);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}