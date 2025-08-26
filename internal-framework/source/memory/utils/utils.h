#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>

namespace Utils
{
	uintptr_t SignatureScan(const char* module, const char* pattern);
	bool Compare(byte* data, byte* sig, uint64_t size);
	byte* FindSignature(byte* start, byte* end, byte* sig, uint64_t size);
	void* FindPointer(HMODULE mod, byte* sig, uint64_t size, int addition);
	uintptr_t GetAbsoluteAddress(uintptr_t address, int offset, int size);
}