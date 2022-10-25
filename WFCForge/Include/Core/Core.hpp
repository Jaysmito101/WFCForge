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
#include <array>
#include <cstdlib>
#include <cstdint>

// Universal Includes
#include "Core/Macros.hpp"
#include "Core/Logger.hpp"
#include "Core/Assert.hpp"
#include "Core/Utils.hpp"
#include "Core/SharedPtr.hpp"

#define WFC_ABRUPT_EXIT(code) { \
		WFC_LOGGER_SHUTDOWN(); \
		exit(code); \
	}