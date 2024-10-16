#pragma once
#include <regex>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>
#include <unordered_set>

#include "memcury.h"
#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/libMinHook.x64.lib")

namespace fs = std::filesystem;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

namespace Patterns 
{
	constexpr const char* VersionPattern_1 = "40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B C8 41 B8 04 ? ? ? 48 8B D3"; /* ch1 - v15.50 */
	constexpr const char* VersionPattern_2 = "2B 2B 46 6F 72 74 6E 69 74 65 2B 52 65 6C 65 61 73 65 2D ? ? ? ?"; /* v15.50 - latest */

	constexpr const char* ExpandAESKey_1 = "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 0F B6 42 ? 48 8B F9";
	constexpr const char* ExpandAESKey_2 = "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 45 33 DB 44 8B D0";
}

namespace Settings
{
	constexpr bool bIsUEFN = true; // enabled by default because you can't dump fn latest anymore
	constexpr bool bShouldSaveFile = true;
	constexpr const char* SavePath = "C:/AESFinder";
}