#include "utils.h"
#include <Psapi.h>

uintptr_t Utils::SignatureScan(const char* module, const char* pattern)
{
	uintptr_t moduleAdress = 0;
	moduleAdress = (uintptr_t)GetModuleHandleA(module);

	static auto patternToByte = [](const char* pattern)
		{
			auto       bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else { bytes.push_back(strtoul(current, &current, 16)); }
			}
			return bytes;
		};

	const auto dosHeader = (PIMAGE_DOS_HEADER)moduleAdress;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdress + dosHeader->e_lfanew);

	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto       patternBytes = patternToByte(pattern);
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleAdress);

	const auto s = patternBytes.size();
	const auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found) { return reinterpret_cast<uintptr_t>(&scanBytes[i]); }
	}
	return NULL;
}

bool Utils::Compare(byte* data, byte* sig, uint64_t size)
{
	for (auto i = 0ull; i < size; i++) { if (data[i] != sig[i] && sig[i] != 0x00) { return false; } }
	return true;
}

byte* Utils::FindSignature(byte* start, byte* end, byte* sig, uint64_t size)
{
	for (auto it = start; it < end - size; it++) { if (Compare(it, sig, size)) { return it; }; }
	return nullptr;
}

void* Utils::FindPointer(HMODULE mod, byte* sig, uint64_t size, int addition)
{
	MODULEINFO info;
	if (!K32GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(MODULEINFO))) { return nullptr; };
	auto base = static_cast<byte*>(info.lpBaseOfDll);
	auto address = FindSignature(base, base + info.SizeOfImage - 1, sig, size);
	if (!address) { return nullptr; }
	auto k = 0;
	for (; sig[k]; k++);
	auto offset = *reinterpret_cast<int*>(address + k);
	return address + k + 4 + offset + addition;
}

uintptr_t Utils::GetAbsoluteAddress(uintptr_t address, int offset, int size) {
	if (address)
	{
		uintptr_t addrs = address + size + *(int*)(address + offset);
		return addrs;
	}
	else
		return NULL;
}