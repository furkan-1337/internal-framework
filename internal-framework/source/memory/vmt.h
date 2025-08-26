#pragma once
#include <cstdint>

namespace VMT
{
    inline void** GetVirtualTable(uintptr_t address)
    {
        return *reinterpret_cast<void***>(address);
    }

    inline void* GetVirtualMethod(uintptr_t address, int index)
    {
        return GetVirtualTable(address)[index];
    }

    template<typename T>
    inline T GetVirtualMethod(uintptr_t address, int index)
    {
        return reinterpret_cast<T>(GetVirtualTable(address)[index]);
    }

    template<typename Ret, typename... Args>
    inline Ret CallVirtualMethod(uintptr_t instance, int index, Args... args)
    {
        auto vtable = GetVirtualTable(instance);
        auto func = reinterpret_cast<Ret(__thiscall*)(void*, Args...)>(vtable[index]);
        return func(reinterpret_cast<void*>(instance), args...);
    }

    inline void* HookMethod(uintptr_t instance, int index, void* newFunc)
    {
        void** vtable = GetVirtualTable(instance);
        void* oldFunc = vtable[index];
        vtable[index] = newFunc;
        return oldFunc;
    }
}