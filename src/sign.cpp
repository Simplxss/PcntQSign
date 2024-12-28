#include "sign.h"

#include <map>
#include <vector>
#include <thread>
#include <stdexcept>

#if defined(_WIN_PLATFORM_)
#define PSAPI_VERSION 1
#include <windows.h>
#include <psapi.h>
#elif defined(_LINUX_PLATFORM_)
#include <dlfcn.h>
#include <link.h>
#include <string.h>
#endif

typedef int (*SignFunctionType)(const char *cmd, const unsigned char *src, size_t src_len, int seq, unsigned char *result);
SignFunctionType SignFunction = nullptr;

// 签名函数定义
#if defined(_X64_ARCH_)
std::map<std::string, uint64_t> addrMap = {
	// Linux
	{"3.1.2-12912", 0x33C38E0},
	{"3.1.2-13107", 0x33C3920},
	{"3.2.7-23361", 0x4C93C57},
	{"3.2.9-24815", 0x4E5D3B7},
	{"3.2.10-25765", 0x4F176D6},
	{"3.2.15-30899", 0x620C4D0},
	// Macos
	{"6.9.19-16183", 0x1B29469},
	// Windows
	{"9.9.2-16183", 0x2E0D0},
	{"9.9.9-23361", 0x2EB50},
	{"9.9.9-23424", 0x2EB50},
	{"9.9.9-23424", 0x2EB50},
	{"9.9.10-24108", 0x2EB50},
	{"9.9.11-24568", 0xAA1A20},
	{"9.9.11-24815", 0xAB5510},
	{"9.9.12-25234", 0xA84980},
	{"9.9.12-25300", 0xA84980},
	{"9.9.12-25493", 0xA996E0},
	{"9.9.12-25765", 0xA9CE90},
	{"9.9.12-26299", 0xA9E5B0},
	{"9.9.12-26299", 0xA9E5B0},
	{"9.9.15-27597", 0xBDAE40},
	{"9.9.17-30899", 0xC32C50}};
#elif defined(_X86_ARCH_)
std::map<std::string, uint64_t> addrMap = {
	// Windows
	{"9.9.2-15962", 0x2BD70},
	{"9.9.2-16183", 0x2BD70}};
#elif defined(_ARM64_ARCH_)
std::map<std::string, uint64_t> addrMap = {
	// Linux
	{"3.2.7-23361", 0x351EC98},
	// Macos
	{"6.9.20-17153", 0x1c73dd0},
	{"6.9.56-28418", 0x23510E8}};
#endif

int SignOffsets = 767; // 562 before 3.1.2-13107, 767 in others
int ExtraOffsets = 511;
int TokenOffsets = 255;

uint64_t Rva2Addr(uint64_t rva)
{
#if defined(_LINUX_PLATFORM_)
	uint64_t baseAddress = 0;

	dl_iterate_phdr([](struct dl_phdr_info *info, size_t size, void *data)
					{
        if (strstr(info->dlpi_name, "wrapper.node") != nullptr) {
			*(uint64_t *)data = (uint64_t)info->dlpi_addr;
			return 1;
        }
        return 0; }, (void *)&baseAddress);

	if (baseAddress)
		return baseAddress + rva;

#elif defined(_WIN_PLATFORM_)
	HMODULE wrapperModule = GetModuleHandleW(L"wrapper.node");
	MODULEINFO modInfo;
	if (wrapperModule != NULL && GetModuleInformation(GetCurrentProcess(), wrapperModule, &modInfo, sizeof(MODULEINFO)))
		return reinterpret_cast<uint64_t>(modInfo.lpBaseOfDll) + rva;

#elif defined(_MAC_PLATFORM_)
	// todo

#endif
	return 0;
}

std::vector<uint8_t> Hex2Bin(std::string_view str)
{
	if (str.length() % 2 != 0)
	{
		throw std::invalid_argument("Hex string length must be even");
	}
	std::vector<uint8_t> bin(str.size() / 2);
	std::string extract("00");
	for (size_t i = 0; i < str.size() / 2; i++)
	{
		extract[0] = str[2 * i];
		extract[1] = str[2 * i + 1];
		bin[i] = std::stoi(extract, nullptr, 16);
	}
	return bin;
}

std::string Bin2Hex(const uint8_t *ptr, size_t length)
{
	const char table[] = "0123456789ABCDEF";
	std::string str;
	str.resize(length * 2);
	for (size_t i = 0; i < length; ++i)
	{
		str[2 * i] = table[ptr[i] / 16];
		str[2 * i + 1] = table[ptr[i] % 16];
	}
	return str;
}

bool Sign::Init(const std::string &version)
{
	uint64_t HookAddress = Rva2Addr(addrMap[version]);
	if (HookAddress == 0)
		throw std::runtime_error("Can't find hook address");
		
	SignFunction = reinterpret_cast<SignFunctionType>(HookAddress);
	return true;
}

std::tuple<std::string, std::string, std::string> Sign::Call(const std::string_view cmd, const std::string_view src, int seq)
{
	if (SignFunction == nullptr)
		throw std::runtime_error("Sign function not initialized");

	const std::vector<uint8_t> signArgSrc = Hex2Bin(src);

	size_t resultSize = 1024;
	uint8_t *signResult = new uint8_t[resultSize];

	SignFunction(cmd.data(), signArgSrc.data(), signArgSrc.size(), seq, signResult);

	std::string signDataHex = Bin2Hex(signResult + 512, *(signResult + SignOffsets));
	std::string extraDataHex = Bin2Hex(signResult + 256, *(signResult + ExtraOffsets));
	std::string tokenDataHex = Bin2Hex(signResult, *(signResult + TokenOffsets));

	delete[] signResult;

	return std::make_tuple(signDataHex, extraDataHex, tokenDataHex);
}
