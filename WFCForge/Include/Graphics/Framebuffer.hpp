#pragma once

#include "Core/Core.hpp"

namespace WFCForge
{

	class Framebuffer
	{
	public:
		Framebuffer();
		virtual ~Framebuffer();

		void Setup(uint32_t width, uint32_t height); // create a new framebuffer
		void Default(); // use the default (handle = 0) framebuffer
		void Destroy();

		void Begin();
		void Clear();
		void End();

		uint32_t ReadMousePickID(uint32_t index, float x, float y);

		inline uint32_t GetHandle() { return this->handle; }
		inline bool IsDefault() { return this->isDefault; }
		inline uint32_t GetColorTex() { WFC_ASSERT(!this->isDefault, "Cannot get attachment IDs for default framebuffer") return this->colorTex; }
		inline uint32_t GetDepthTex() { WFC_ASSERT(!this->isDefault, "Cannot get attachment IDs for default framebuffer") return this->depthTex; }
		inline uint32_t GetMousePickTex(uint32_t index) { WFC_ASSERT(!this->isDefault, "Cannot get attachment IDs for default framebuffer") WFC_ASSERT(index >= 0 && index < WFC_ARRAY_SIZE(mousePickTex), "Invalid texture index") return this->mousePickTex[index]; }

	private:
		uint32_t width = 512;
		uint32_t height = 512;
		bool isDefault = false;
		uint32_t handle = 0;
		uint32_t colorTex = 0;
		uint32_t depthTex = 0;
		uint32_t mousePickTex[3] = {0, 0, 0};
	};

}
