#include "globals.h"
#include "FAESKey.h"

/* utility for convert a pointer to an hex */
static inline uintptr_t PointerToHex(void* ptr)
{
	return reinterpret_cast<uintptr_t>(ptr);
}

static inline bool CreateOutputFolder()
{
	fs::path filePath(Settings::SavePath);
	try
	{
		if (!fs::exists(filePath)) {
			fs::create_directories(filePath);
		}
	}
	catch (const fs::filesystem_error& e)
	{
		std::string error(std::format("Could not create the required folder: {}. Using the game one.", e.what()));
		std::cout << error << "\n";
		MessageBoxA(nullptr, (error.c_str()), ("AESFinder"), MB_ICONERROR);
		return false;
	}

	return true;
}

/* current version & build of the game for the dump */
inline std::string FNBuild;
inline std::string FNVersion;

/* NOTE: this is supported only from v15.50 to latest */
static inline bool GetFNVersion()
{
	uintptr_t FNBuild_Addr = Memcury::Scanner::FindPattern(Patterns::VersionPattern_2).Get();
	if (!FNBuild_Addr) {
		FNVersion = "??";
		FNBuild = "??";
		return false;
	}

	FNBuild = decltype(FNBuild.c_str())(FNBuild_Addr);
	std::regex pattern(R"((\d+\.\d+))");
	std::smatch match;
	std::regex_search(FNBuild, match, pattern);
	FNVersion = "v" + std::string(match[1]);

	return true;
}

/* pointer to the original FAES::ExpandAESKey() function */
inline uint64(*OriginalExpandAESKey)(uint8* Contents, FAESKey& Key) = nullptr;

uint64 ExpandAESKeyHook(uint8* Contents, FAESKey& Key)
{
	static std::unordered_set<std::string> UniqueKeys;
	std::string key = Key.ToString();
	uintptr_t keyAddr = PointerToHex(&Key);

	auto [_, bInserted] = UniqueKeys.insert(key);
	if (bInserted) {
		std::cout << "\n\n\n(Address: 0x" << std::hex << keyAddr << ") AES-Key: " << key << "\n\n\n";

		if (Settings::bShouldSaveFile) {
			fs::path savePath;
			if (CreateOutputFolder()) {
				savePath = fs::path(Settings::SavePath) / ("AESKey_" + FNVersion + ".txt");
			} 
			else {
				savePath = "AESKey_" + FNVersion + ".txt";
			}

			std::ofstream file(savePath);
			file << "AESFinder - Originally created by Fischsalat, reworked by djlorenzouasset.\n\n";
			file << "Game Build: " << FNBuild << "\n";
			file << "Game Version: " << FNVersion << "\n";
			file << "AES key: " << key << "\n";
			file << "Address: 0x" << std::hex << keyAddr << "\n";
			file << "BaseAddress: 0x" << std::hex << PointerToHex(GetModuleHandle(0)) << "\n";
			file.close();
		}
	}

	/* this is always 14 (0xE), the number of Rounds required by the decryption algorithm */
	return OriginalExpandAESKey(Contents, Key);
}

DWORD MainThread(HMODULE hModule)
{
	AllocConsole();

	FILE* out;
	freopen_s(&out, "CONOUT$", "w", stdout);
	freopen_s(&out, "CONIN$", "r", stdin);

	if (!GetFNVersion()) {
		std::cout << R"(FN Version could not be found. Default will be "??")" << "\n";
	}

	std::cout << "[+] AESFinder - Originally created by Fischsalat, reworked by djlorenzouasset.\n";
	std::cout << "[+] BaseAddress is: 0x" << std::hex << PointerToHex(GetModuleHandle(0)) << "\n";
	std::cout << "[+] Current game version is: " << FNVersion << std::format(" ({})", FNBuild) << "\n";

	if (MH_Initialize() != MH_STATUS::MH_OK) {
		MessageBoxA(nullptr, ("Error initializing MinHook, terminating."), ("AESFinder"), MB_ICONERROR);
		return 0;
	}

	/* gets the address to the FAES::ExpandAesKey function */
	std::cout << "[+] Scanning for " << Patterns::ExpandAESKey_1 << "\n";

	void* ExpandAESKey_Addr = Memcury::Scanner::FindPattern(Patterns::ExpandAESKey_1).GetAs<void*>();
	if (!ExpandAESKey_Addr) {
		/* if the first pattern is nullptr, lets try the second one */
		std::cout << "Function not found, trying to scan " << Patterns::ExpandAESKey_2 << "\n";
		ExpandAESKey_Addr = Memcury::Scanner::FindPattern(Patterns::ExpandAESKey_2).GetAs<void*>();
	}

	std::cout << "[+] Found FAES::ExpandAESKey() function at 0x" << std::hex << PointerToHex(ExpandAESKey_Addr) << "\n";

	/* CREATES THE HOOK */
	MH_STATUS CreateHook = MH_CreateHook(ExpandAESKey_Addr, ExpandAESKeyHook, reinterpret_cast<void**>(&OriginalExpandAESKey));
	if (CreateHook != MH_STATUS::MH_OK) {
		MessageBoxA(nullptr, ("Can't create hook to the FAES::ExpandAESKey() function!"), ("AESFinder"), MB_ICONERROR);
		return 0;
	}

	/* ENABLES THE HOOK */
	MH_STATUS EnableHook = MH_EnableHook(ExpandAESKey_Addr);
	if (CreateHook != MH_STATUS::MH_OK) {
		MessageBoxA(nullptr, ("Can't enable hook to the FAES::ExpandAESKey() function!"), ("AESFinder"), MB_ICONERROR);
		return 0;
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
	}

	return TRUE;
}