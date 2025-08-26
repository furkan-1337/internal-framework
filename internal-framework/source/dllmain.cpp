#include <Windows.h>
#include <process.h>
#include "debug/debug.h"
#include "memory/hook/hook_engine.h"
#include "render/direct_hook.h"

extern void Load(HMODULE hModule);
extern void Unload(HMODULE hModule);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Load(hModule);
        break;
    case DLL_PROCESS_DETACH:
        Unload(hModule);
        break;
    }
    return TRUE;
}

unsigned int __stdcall MainThread(void* lpParam)
{
    Debug::SetDebugMode(true);
    if (Debug::Check("Hook Engine", HookEngine::Initialize()))
    {
        if (!Debug::Check("DirectX", DirectHook::Setup()))
            return 0;
    }
    return 0;
}

void Load(HMODULE hModule)
{
    DisableThreadLibraryCalls(hModule);

    uintptr_t thread = _beginthreadex(
        nullptr,
        0,
        MainThread,
        nullptr,
        0,
        nullptr
    );

    if (thread)
        CloseHandle(reinterpret_cast<HANDLE>(thread));
}

void Unload(HMODULE hModule)
{
    FreeLibraryAndExitThread(hModule, 0);
}
