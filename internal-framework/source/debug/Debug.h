#pragma once
#include <Windows.h>

namespace Debug
{
	inline bool IsDebugMode = false;

	bool IsConsoleAttached();
	void SetDebugMode(bool enable);
	bool Check(const char* Name, bool result);
	void Print(const char* format, ...);
}