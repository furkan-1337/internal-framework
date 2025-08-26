#include "Debug.h"
#include <cstdio>

bool Debug::IsConsoleAttached()
{
	return GetConsoleWindow() != NULL;
}

void Debug::SetDebugMode(bool enable)
{
    if (enable)
    {
        AllocConsole();

        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONIN$", "r", stdin);
        freopen_s(&fp, "CONOUT$", "w", stderr);

        SetConsoleTitleA("[internal-framework] console");
        IsDebugMode = true;
    }
    else
    {
        if (IsConsoleAttached())
            FreeConsole();
        IsDebugMode = false;
    }
}


bool Debug::Check(const char* name, bool result)
{
    Debug::Print(result ? "%s initialized successfully!" : "%s failed to initialize!", name);
    return result;
}

void Debug::Print(const char* format, ...)
{
    if (!(IsDebugMode && IsConsoleAttached()))
        return;

    if (!format)
        return;

    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("[+] %s\n", buffer);
}