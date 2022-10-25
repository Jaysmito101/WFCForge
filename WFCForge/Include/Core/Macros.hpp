#pragma once

// Version
#define WFC_VERSION_MAX 1
#define WFC_VERSION_MIN 0


// OS Detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WFC_WINDOWS
#ifdef _WIN64
#define WFC_X64
#else
#define WFC_X86
#error "32-bit builds are not officially supported"
#endif
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#error "Platform not supported"
#elif TARGET_OS_MACCATALYST
#error "Platform not supported"
#elif TARGET_OS_IPHONE
#error "Platform not supported"
#elif TARGET_OS_MAC
#define VK3D_MAC
#else
#error "Unknown Apple platform"
#endif
#elif __ANDROID__
#error "Android builds are not supported"
#elif __linux__
#define VK3D_LINUX
#elif __unix__ // all unices not caught above
#error "Platform not supported"
#elif defined(_POSIX_VERSION)
#error "Platform not supported"
#else
#error "Unknown compiler"
#endif

// Debug/Release Detection
#ifdef NDEBUG
#define WFC_RELEASE
#else
#define WFC_DEBUG
#endif

// OS Specific macros

#ifdef  WFC_WINDOWS
#define PATH_SEPERATOR "\\"
#define DEBUG_BREAK __debugbreak();

#else
#define PATH_SEPERATOR "/"
#define DEBUG_BREAK throw std::runtime_error("DEBUG BREAK");

#endif 

// Utils
#define WFC_ARRAY_SIZE(arr) ( sizeof(arr) / sizeof(arr[0]) )
#define WFC_SAFE_DELETE(x) if(x) { delete x; x = nullptr; }

#define WFC_HANDLE_EXCEPTION_MSG(x, message) { try {x;} catch(std::exception& e){WFC_LOG_ERROR("%s [ %s ]", message, e.what());} }
#define WFC_HANDLE_EXCEPTION(x) WFC_HANDLE_EXCEPTION_MSG(x, "Exception:")

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
