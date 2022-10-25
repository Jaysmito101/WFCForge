#pragma once

#include "Core/Core.hpp"

#include <glm/glm.hpp>

namespace WFCForge
{
	struct Vertex
	{
		glm::vec4 position = glm::vec4(0.0f);
		glm::vec4 normal = glm::vec4(0.0f);
		glm::vec4 texCoord = glm::vec4(0.0f);

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& nor)
		{
			this->position = glm::vec4(pos.x, pos.y, pos.z, 0.0f);
			this->normal = glm::vec4(nor.x, nor.y, nor.z, 0.0f);
			this->texCoord = glm::vec4(tex.x, tex.y, 0.0f, 0.0f);
		}

		Vertex() 
		{
		}
	};

	class Mesh
	{
	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		void Setup(uint32_t vertexCount, const Vertex* vertices, uint32_t indexCount, const uint32_t* indices);
		void Destroy();

		void Render();
		void RenderWireFrame();
		void RenderInstanced(uint32_t count);
		void RenderWireFrameInstanced(uint32_t count);

		inline bool IsSetup() { return this->vao != 0; }

	private:
		uint32_t vao = 0;
		uint32_t vbo = 0;
		uint32_t ebo = 0;
		uint32_t inc = 0;

	};

}
