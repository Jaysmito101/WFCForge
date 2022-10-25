#include "Core/Utils.hpp"
#include "Core/Macros.hpp"
#include "Core/Logger.hpp"

#ifdef WFC_WINDOWS

#include <Windows.h>

#elif defined(WFC_LINUX)

#elif defined(WFC_MAC)

#endif

namespace WFCForge
{

	namespace Utils
	{



		std::string GetExecutablePath()
		{
			char rawPathName[MAX_PATH];
#ifdef WFC_WINDOWS
			GetModuleFileNameA(NULL, rawPathName, MAX_PATH);
#else
			readlink("/proc/self/exe", rawPathName, PATH_MAX);
#endif
			return std::string(rawPathName);
		}

		std::string GetExecutableDir()
		{
			std::string executablePath = GetExecutablePath();
			std::string directory = executablePath.substr(0, executablePath.find_last_of("\\/"));
			return directory;
		}

		std::string ReadFile(const std::string& path, bool* loaded)
		{
			try {
				std::ifstream f;
				f.open(path);

				if (f.is_open() && f.good())
				{
					std::stringstream s;
					s << f.rdbuf();
					f.close();
					if (loaded)
						*loaded = true;
					return s.str();
				}
				else
				{
					if (loaded)
						*loaded = false;
					return "";
				}
			}
			catch (std::exception& e)
			{
				WFC_LOG_ERROR("Error while reading text file %s, ERROR: %s", path.data(), e.what());
				if (loaded)
					*loaded = false;
				return "";
			}
		}

		bool PathExists(const std::string& path)
		{
			return std::filesystem::exists(path);
		}

		bool WriteFile(const std::string& path, const std::string& data)
		{
			std::fstream file;
			file.open(path);
			if (file.is_open())
			{
				file << data;
				file.close();
				return true;
			}
			return false;
		}

		bool AppendFile(const std::string& path, const std::string& data)
		{
			std::fstream file;
			file.open(path, std::ios_base::app);
			if (file.is_open())
			{
				file << data;
				file.close();
				return true;
			}
			return false;
		}

		void SleepFor(uint64_t duration)
		{
#ifdef WFC_WINDOWS
			Sleep((DWORD)duration);
#else
#error "Not yet implemented"
#endif
		}

		uint32_t Hash(const void* dat, size_t len)
		{
    		const uint8_t* data = (const uint8_t*)dat;
    		uint32_t hash = (uint32_t)len;
    		uint32_t tmp;
    		int rem;

    		if (len <= 0 || data == NULL) return 0;

    		rem = len & 3;
    		len >>= 2;

    		/* Main loop */
    		for (;len > 0; len--) {
        		hash  += CGL_get16bits (data);
        		tmp    = (CGL_get16bits (data+2) << 11) ^ hash;
        		hash   = (hash << 16) ^ tmp;
        		data  += 2*sizeof (uint16_t);
        		hash  += hash >> 11;
    		}

    		/* Handle end cases */
    		switch (rem) {
        		case 3: hash += CGL_get16bits (data);
                		hash ^= hash << 16;
                		hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                		hash += hash >> 11;
                		break;
        		case 2: hash += CGL_get16bits (data);
                		hash ^= hash << 11;
                		hash += hash >> 17;
                		break;
        		case 1: hash += (signed char)*data;
                		hash ^= hash << 10;
                		hash += hash >> 1;
    		}

    		/* Force "avalanching" of final 127 bits */
    		hash ^= hash << 3;
    		hash += hash >> 5;
    		hash ^= hash << 4;
    		hash += hash >> 17;
    		hash ^= hash << 25;
    		hash += hash >> 6;

    		return hash;
		}


	}

}