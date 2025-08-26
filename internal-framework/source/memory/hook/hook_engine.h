#pragma once

namespace HookEngine
{
	inline bool IsInitialized = false;
	bool Initialize();
	bool Uninitialize();
	bool Hook(void* targetAddress, void* function, void** original);
	bool Unhook(void* targetAddress);
}