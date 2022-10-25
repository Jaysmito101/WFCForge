#pragma once

#include "Core/Logger.hpp"
#include "Core/Macros.hpp"

#ifdef WFC_DEBUG
#define WFC_ASSERT(condition, ...) \
	if(!(condition)) \
	{ \
		WFC_LOG_ERROR(__VA_ARGS__); \
		WFC_LOG_ERROR("ASSERTION FAILED\n File: %s\n\tLine: %d\n\tFunction: %s",  __FILE__, __LINE__, __PRETTY_FUNCTION__); \
		WFCForge::Logger::Shutdown(); \
		DEBUG_BREAK; \
	}
#else
#define WFC_ASSERT(condition, ...)
#endif

