#include "Graphics/BatchRenderer.hpp"

#include <glad/glad.h>

namespace WFCForge
{

	const std::string batchRendererVertexShaderSource = R"(
#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 color;

out vec3 Position;
out vec3 Normal;
out vec3 Color;

uniform mat4 uPV;

void main()
{
	gl_Position = uPV * position;
	Position = position.xyz;
	Normal =  normal.xyz;
	Color = color.xyz;
}
)";

	const std::string batchRendererFragmentShaderSource = R"(
#version 430 core

out vec4 FragColor;
in vec3 Position;
in vec3 Normal;
in vec3 Color;

uniform vec3 uLightPosition;
uniform vec3 uCameraPosition;

const vec3 lightColor = vec3(0.2f); 

void main()
{
	vec3 ambient = lightColor * Color;

	vec3 lightDir = normalize(uLightPosition - Position);
	float diff = max(dot(lightDir, Normal), 0);
	vec3 diffuse = Color * diff * 2.0f;

	FragColor = vec4(  ambient + diffuse , 1.0f);
}
)";

	void BatchRenderer::Setup()
	{
		this->vertices.reserve(600);
		this->shader.Build(batchRendererVertexShaderSource, batchRendererFragmentShaderSource);
		
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(BatchRendererVertex), (void*)offsetof(BatchRendererVertex, color));
		glEnableVertexAttribArray(2);
	}
	
	void BatchRenderer::Destroy()
	{
		this->shader.Destroy();

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
	
	void BatchRenderer::Begin(const glm::mat4& pv, const glm::vec3 lightPos, const glm::vec3& cameraPos)
	{
		this->pv = pv;
		this->lightPos = lightPos;
		this->cameraPos = cameraPos;
		this->vertices.clear();
	}
	
	void BatchRenderer::End()
	{
		this->Flush();
	}
	
	void BatchRenderer::Cube(glm::vec3 start, glm::vec3 size, glm::vec3 color)
	{
		start -= size * glm::vec3(-0.5f, -0.5f, -0.5f);
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, -0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, 0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, 0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));	
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, -0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, 0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, 0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, -0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, 0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, 0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, 0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));		
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, -0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(-0.5f, -0.5f, 0.5f),color, glm::vec3(0.0f, 0.0f, 0.0f)));
		this->Vertex(BatchRendererVertex(start + size * glm::vec3(0.5f, -0.5f, 0.5f),	color, glm::vec3(0.0f, 0.0f, 0.0f)));


	}

	void BatchRenderer::Vertex(const BatchRendererVertex& vert)
	{
		this->vertices.push_back(vert);
		if (this->vertices.size() % 3 == 0)
		{
			auto& a = this->vertices[this->vertices.size() - 3];
			auto& b = this->vertices[this->vertices.size() - 2];
			auto& c = this->vertices[this->vertices.size() - 1];
			auto ab = glm::vec3(b.position - a.position);
			auto ac = glm::vec3(c.position - a.position);
			auto normal = glm::cross(ab, ac);
			a.normal = glm::vec4(normal, 1.0f);
			b.normal = glm::vec4(normal, 1.0f);
			c.normal = glm::vec4(normal, 1.0f);
		}
	}
	
	void BatchRenderer::Flush()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(BatchRendererVertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
		glm::mat4 mat = glm::translate(glm::mat4(1.0f), this->position);
		mat = glm::rotate(mat, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		mat = glm::rotate(mat, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		mat = pv * mat;
		shader.Bind();
		shader.SetMatrix4(Uniform_uPV, mat);
		shader.SetVector3(Uniform_uLightPosition, lightPos);
		shader.SetVector3(Uniform_uCameraPosition, cameraPos);
		if(vertices.size() > 0) glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
		vertices.clear();
	}
}