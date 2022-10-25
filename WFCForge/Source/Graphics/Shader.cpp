#include "Core/Core.hpp"
#include "Graphics/Shader.hpp"

#include <glad/glad.h>

namespace WFCForge
{

	static int CompileShader(const std::string& shaderSrc, GLenum shaderType, const std::string& name)
	{
		GLuint shader = glCreateShader(shaderType);
		const GLchar* source = (const GLchar*)shaderSrc.data();
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			char* errorLog = new char[maxLength];
			errorLog[0] = '\0';
			glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
			WFC_LOG_ERROR("Shader Compilation Failed (%s) : \n%s", name.data(), errorLog);
			delete[] errorLog;
			glDeleteShader(shader);
			return -1;
		}

		return shader;
	}


	bool Shader::Build(const std::string& vertexSource, const std::string& fragmentSource)
	{
		WFC_ASSERT(!this->handle, "Shader already built");
		GLuint vertShader = CompileShader(vertexSource, GL_VERTEX_SHADER, "Vertex");
		GLuint fragShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER, "Fragment");
		if (!vertShader || !fragShader)
			return false;
		this->handle = glCreateProgram();
		glAttachShader(this->handle, vertShader);
		glAttachShader(this->handle, fragShader);
		glLinkProgram(this->handle);
		GLint isLinked = 0;
		glGetProgramiv(this->handle, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(this->handle, GL_INFO_LOG_LENGTH, &maxLength);
			char* errorLog = new char[maxLength];
			errorLog[0] = '\0';
			glGetProgramInfoLog(this->handle, maxLength, &maxLength, errorLog);
			glDeleteProgram(this->handle);
			glDeleteShader(vertShader);
			glDeleteShader(fragShader);
			WFC_LOG_ERROR("Shader Linking Failed : \n%s", errorLog);
			delete[] errorLog;
			return false;
		}

		glDetachShader(this->handle, vertShader);
		glDetachShader(this->handle, fragShader);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		this->isBuild = true;

		this->LoadUniforms();


		return true;
	}

	void Shader::Destroy()
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glDeleteProgram(this->handle);
		this->isBuild = false;
	}

	void Shader::Bind()
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUseProgram(this->handle);
	}

	void Shader::SetFloat(Uniform name, const float data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform1f(this->uniformLocations[name], data);
	}

	void Shader::SetInteger(Uniform name, const int data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform1i(this->uniformLocations[name], data);
	}

	void Shader::SetIVector2(Uniform name, const int* data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform2iv(this->uniformLocations[name], 2, data);
	}

	void Shader::SetIVector3(Uniform name, const int* data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform3iv(this->uniformLocations[name], 3, data);
	}

	void Shader::SetIVector4(Uniform name, const int* data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform4iv(this->uniformLocations[name], 4, data);
	}

	void Shader::SetVector2(Uniform name, const glm::vec2& data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform2f(this->uniformLocations[name], data.x, data.y);
	}

	void Shader::SetVector3(Uniform name, const glm::vec3& data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform3f(this->uniformLocations[name], data.x, data.y, data.z);
	}

	void Shader::SetVector4(Uniform name, const glm::vec4& data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniform4f(this->uniformLocations[name], data.x, data.y, data.z, data.w);
	}

	void Shader::SetMatrix3(Uniform name, const glm::mat3& data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniformMatrix3fv(this->uniformLocations[name], 1, GL_FALSE, glm::value_ptr(data));
	}

	void Shader::SetMatrix4(Uniform name, const glm::mat4& data)
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");
		glUniformMatrix4fv(this->uniformLocations[name], 1, GL_FALSE, glm::value_ptr(data));
	}

	void Shader::LoadUniforms()
	{
		WFC_ASSERT(this->isBuild, "Shader not yet build");

		for (int i = 0; i < Uniform_Count; i++)
		{
			uniformLocations[i] = glGetUniformLocation(this->handle, (const GLchar*)UniformNames[i]);
		}
	}

}