#pragma once
#include "Core/Core.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Graphics/Shader.hpp"

namespace WFCForge
{
	struct BatchRendererVertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 color;

		BatchRendererVertex(glm::vec3 a, glm::vec3 c, glm::vec3 b)
			: position(a.x, a.y, a.z, 1.0f), normal(b.x, b.y, b.z, 1.0f), color(c.x, c.y, c.z, 1.0f)
		{}
	};

	class BatchRenderer
	{
	public:
		BatchRenderer() = default;
		virtual ~BatchRenderer() = default;

		void Setup();
		void Destroy();

		void Begin(const glm::mat4& pv, const glm::vec3 lightPos, const glm::vec3& cameraPos);
		void End();

		void Cube(glm::vec3 start, glm::vec3 size, glm::vec3 color);
		void Vertex(const BatchRendererVertex& vert);

		inline float* GetRotationPTR() { return glm::value_ptr(this->rotation); }
		inline float* GetPositionPTR() { return glm::value_ptr(this->position); }

	private:
		void Flush();


	private:
		std::vector<BatchRendererVertex> vertices;
		uint32_t vbo = 0, vao = 0;
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 position = glm::vec3(0.0f);
		Shader shader;
		glm::mat4 pv = glm::mat4(1.0f);
		glm::vec3 lightPos = glm::vec3(0.0f, 2.0f, 0.0f);
		glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 0.0f);
	};
}