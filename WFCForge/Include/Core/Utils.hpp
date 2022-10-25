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

namespace WFCForge
{

	namespace Utils
	{

		std::string GetExecutablePath();
		std::string GetExecutableDir();

		std::string ReadFile(const std::string& path, bool* loaded = nullptr);

		bool PathExists(const std::string& path);
		bool WriteFile(const std::string& path, const std::string& data);
		bool AppendFile(const std::string& path, const std::string& data);

		void SleepFor(uint64_t duration);

	}

}