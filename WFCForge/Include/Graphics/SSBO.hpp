#pragma once
#include "Core/Core.hpp"

namespace  WFCForge
{

	class SSBO
	{
	public:
		SSBO() = default;
		virtual ~SSBO() = default;

		void Setup(size_t size, uint32_t binding = 0);
		void Destroy();
		void Resize(size_t newSize);
		void SetData(void* data, size_t size, size_t offset = 0);
		void GetData(void* data, size_t size, size_t offset = 0);

		inline bool IsSetup() { return this->isSetup; }
		inline size_t GetSize() { return this->size; }

	private:
		bool isSetup = false;
		uint32_t handle = 0;
		uint32_t binding = 0;
		size_t size = 0;
	};


}
