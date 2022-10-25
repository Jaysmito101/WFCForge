#include "Core/Core.hpp"
#include "Graphics/SSBO.hpp"

#include <glad/glad.h>

namespace WFCForge
{

	void SSBO::Setup(size_t sz, uint32_t bind)
	{
		WFC_ASSERT(!this->isSetup, "SSBO already setup");
		this->size = sz;
		this->binding = bind;

		glGenBuffers(1, &this->handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, this->size, NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, this->handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		this->isSetup = true;
	}

	void SSBO::Resize(size_t newSize)
	{
		WFC_ASSERT(this->isSetup, "SSBO not yet setup");
		this->size = newSize;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, this->size, NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, this->handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void SSBO::SetData(void* data, size_t lSize, size_t lOffset)
	{
		WFC_ASSERT(this->isSetup, "SSBO not yet setup");
		WFC_ASSERT(lSize <= this->size, "Invalid size");
		WFC_ASSERT(lOffset >= 0, "Invalid offset");
		WFC_ASSERT((lOffset + lSize) <= this->size, "Invalid offset");

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->handle);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, lOffset, lSize, data);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, this->handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	}

	void SSBO::GetData(void* data, size_t lSize, size_t lOffset)
	{
		WFC_ASSERT(this->isSetup, "SSBO not yet setup");		
		WFC_ASSERT(lSize <= this->size, "Invalid size");
		WFC_ASSERT(lOffset >= 0, "Invalid offset");
		WFC_ASSERT((lOffset + lSize) <= this->size, "Invalid offset");

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->handle);
		glGetNamedBufferSubData(this->handle, lOffset, lSize, data);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void SSBO::Destroy()
	{
		WFC_ASSERT(this->isSetup, "SSBO not yet setup");
		glDeleteBuffers(1, &this->handle);
		this->handle = 0;
		this->isSetup = false;
	}

}