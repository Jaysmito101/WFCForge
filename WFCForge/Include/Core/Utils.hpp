#pragma once

// STD Includes
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <queue>
#include <format>
#include <iostream>
#include <fstream>
#include <functional>
#include <cstdint>
#include <filesystem>

#include <glm/glm.hpp>

namespace WFCForge
{

	namespace Utils
	{

		std::string GetExecutablePath();
		std::string GetExecutableDir();

		std::string ShowSaveFileDialog();
		std::string ShowFileOpenDialog();

		std::string ReadFile(const std::string& path, bool* loaded = nullptr);

		bool PathExists(const std::string& path);
		bool WriteFile(const std::string& path, const std::string& data);
		bool AppendFile(const std::string& path, const std::string& data);

		void SleepFor(uint64_t duration);

		uint32_t Hash(void* data, size_t size);
		std::string ToHexString(const unsigned char* data, size_t size);

		inline float Random(float min = 0.0f, float max = 1.0f) { return ((float)rand() / RAND_MAX) * (max - min) + min; }

		inline glm::vec3 Random(const glm::vec3& preffered, const glm::vec3& lim)
		{
			return glm::vec3(
				preffered.x + Random(-lim.x, lim.x) * 0.5f,
				preffered.y + Random(-lim.y, lim.y) * 0.5f,
				preffered.z + Random(-lim.z, lim.z) * 0.5f
			);
		}

		template <typename T>
		inline T Random(const std::vector<T>& vec)
		{
			return vec[rand() % vec.size()];
		}

	}

}