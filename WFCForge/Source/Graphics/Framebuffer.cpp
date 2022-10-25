#include "Core/Core.hpp"
#include "Graphics/Framebuffer.hpp"

#include <glad/glad.h>

namespace WFCForge
{



	Framebuffer::Framebuffer()
	{
	}

	Framebuffer::~Framebuffer()
	{
		if (this->handle || isDefault)
			Destroy();
	}

	void Framebuffer::Setup(uint32_t w, uint32_t h)
	{
		WFC_ASSERT(!this->handle && !this->isDefault, "Framebuffer already Setup");

		this->width = w;
		this->height = h;

		glGenFramebuffers(1, &this->handle);
		glBindFramebuffer(GL_FRAMEBUFFER, this->handle);

		// setup attachments
		glGenTextures(1, &this->colorTex);
		glGenTextures(1, &this->depthTex);
		glGenTextures(3, this->mousePickTex);

		// the main color texture
		glBindTexture(GL_TEXTURE_2D, this->colorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTex, 0);

		// the mouse pick textures
		for (int i = 0; i < 3; i++)
		{
			glBindTexture(GL_TEXTURE_2D, this->mousePickTex[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, this->width, this->height, 0, GL_RED_INTEGER, GL_INT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1 + i, GL_TEXTURE_2D, this->mousePickTex[i], 0);
		}

		// the depth texture
		glBindTexture(GL_TEXTURE_2D, this->depthTex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, this->width, this->height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->depthTex, 0);

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, buffers);

		WFC_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		// Unbind the newly created framebuffer object
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void Framebuffer::Default()
	{
		WFC_ASSERT(!this->handle && !this->isDefault, "Framebuffer already Setup");
		this->isDefault = true;
		this->handle = 0;
	}

	void Framebuffer::Destroy()
	{
		if (this->isDefault)
			this->isDefault = false;
		else
		{
			glDeleteFramebuffers(1, &this->handle);
			glDeleteTextures(1, &this->colorTex);
			glDeleteTextures(1, &this->depthTex);
			glDeleteTextures(3, this->mousePickTex);
			this->handle = 0;
		}
	}

	void Framebuffer::Begin()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
		if (this->isDefault)
		{
			// TODO: Get Framebuffer size from GLFW
		}
		glViewport(0, 0, this->width, this->height);
	}

	void Framebuffer::Clear()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	void Framebuffer::End()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t Framebuffer::ReadMousePickID(uint32_t index, float x, float y)
	{
		WFC_ASSERT(index < 3, "Invalid index");
		glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + 1 + index);
		int32_t pixelData;
		int px = static_cast<int>(x * (this->width - 1));
		int py = static_cast<int>((1.0f - y) * (this->height - 1));
		glReadPixels(px, py, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;

	}

}