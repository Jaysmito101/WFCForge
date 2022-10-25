#pragma once
#include "Core/Core.hpp"

namespace WFCForge
{

	class Texture2D
	{
	public:
		Texture2D() = default;
		virtual ~Texture2D() = default;

		bool LoadFromFile(const std::string& path);
		void LoadFromMemory(const uint32_t width, const uint32_t height, const unsigned char* data);
		void Destroy();

		void Bind(uint32_t slot);

		inline bool IsLoaded() { return this->isLoaded; }
		inline uint32_t GetHandle() { return this->handle; }
		inline uint32_t GetWidth() { return this->width; }
		inline uint32_t GetHeight() { return this->height; }
		inline std::pair<uint32_t, uint32_t> GetSize() { return { this->width, this->height }; }

	private:
		bool isLoaded = false;
		uint32_t handle = 0;
		uint32_t width = 0;
		uint32_t height = 0;
	};

}
