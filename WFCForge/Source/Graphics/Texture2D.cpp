#include "Core/Core.hpp"
#include "Graphics/Texture2D.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

namespace WFCForge
{

	bool Texture2D::LoadFromFile(const std::string& path)
	{

		WFC_ASSERT(!this->isLoaded, "Texture already loaded");
		
		int h = 0;
		int w = 0;
		int c = 0;
		unsigned char* data = stbi_load(path.data(), &w, &h, &c, STBI_rgb_alpha);
		
		if (!data)
		{
			WFC_LOG_ERROR("Failed to load texture (%s)", path.data());
			return false;
		}
		this->LoadFromMemory(w, h, data);
		stbi_image_free(data);

		isLoaded = true;


		return true;
	}

	void Texture2D::LoadFromMemory(const uint32_t w, const uint32_t h, const unsigned char* data)
	{
		WFC_ASSERT(!this->isLoaded, "Texture already loaded");

		this->width = w;
		this->height = h;
		glGenTextures(1, &this->handle);
		glBindTexture(GL_TEXTURE_2D, this->handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);
		isLoaded = true;
	}

	void Texture2D::CreateEmpty(const uint32_t width, const uint32_t height)
	{
		WFC_ASSERT(!this->isLoaded, "Texture already loaded");

		this->width = width;
		this->height = height;
		glGenTextures(1, &this->handle);
		glBindTexture(GL_TEXTURE_2D, this->handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);
		isLoaded = true;
	}
	
	void Texture2D::UploadData(const uint32_t offsetX, const uint32_t offsetY, const uint32_t sizeX, const uint32_t sizeY, void* data)
	{
		glBindTexture(GL_TEXTURE_2D, this->handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, sizeX, sizeY, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Destroy()
	{
		WFC_ASSERT(this->isLoaded, "Texture not yet loaded");

		glDeleteTextures(1, &this->handle);

		isLoaded = false;
	}

	void Texture2D::Bind(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, this->handle);
	}


}