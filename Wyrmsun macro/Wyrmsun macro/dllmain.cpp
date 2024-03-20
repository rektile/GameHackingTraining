// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <vector>
#include "HackValues.h"
#include <iostream>

DWORD base;
DWORD* foodPointer;
DWORD* maxFoodPointer;
DWORD* moneyPointer;
DWORD* copperPointer;
DWORD* stonePointer;

DWORD* unitBaseList;
DWORD* unitBase;
const unsigned int unitDataSize = 0x150;
unsigned char unitdata[unitDataSize];
bool init = false;

DWORD recruitCreateFunctionAddr;
DWORD recruitUnitRetAddress;
DWORD gameLoopRetAddress;

uintptr_t GetAddressFromOffsets(uintptr_t startAddress, std::vector<unsigned int> offsets)
{

    uintptr_t addr = startAddress;
    for (unsigned int i = 0; i < offsets.size(); i++)
    {
        addr = *(uintptr_t*)addr;
        addr += offsets[i];
    }

    return addr;

}


__declspec(naked) void RecruitUnitCodecave()
{
    __asm
    {
        pushad
        mov unitBaseList, ecx
    }

    unitBase = (DWORD*)(*unitBaseList);
    memcpy(unitdata, unitBase, unitDataSize);
    init = true;

    __asm
    {
        popad
        push ecx
        mov ecx, esi
        call recruitCreateFunctionAddr
        jmp recruitUnitRetAddress
    }

}

__declspec(naked) void GameloopCodecave()
{
    __asm
    {
        pushad
    }

    if (init && *copperPointer > 3000 && *maxFoodPointer > *foodPointer)
    {
        memcpy(unitBase, unitdata, unitDataSize);
        __asm
        {
            push 0 // amount ?
            mov ecx, unitBaseList
            push ecx
            call recruitCreateFunctionAddr
        }
    }

    __asm
    {
        popad
        jmp gameLoopRetAddress
    }
}

void InitPointers()
{
    moneyPointer = (DWORD*)GetAddressFromOffsets(base + HackValues::moneyBase, HackValues::moneyOffsets);
    foodPointer = (DWORD*)GetAddressFromOffsets(base + HackValues::foodBase, HackValues::foodOffsets);
    maxFoodPointer = (DWORD*)GetAddressFromOffsets(base + HackValues::foodBase, HackValues::maxFoodOffsets);
    copperPointer = (DWORD*)GetAddressFromOffsets(base + HackValues::copperBase, HackValues::copperOffsets);
    stonePointer = (DWORD*)GetAddressFromOffsets(base + HackValues::stoneBase, HackValues::stoneOffsets);
}

void CreateGameLoopHook()
{
    unsigned char* hookLocation = (unsigned char*)(base + HackValues::gameLoopHookLocationOffset);
    gameLoopRetAddress = (DWORD)hookLocation + 5;
    DWORD oldProtect;
    VirtualProtect((void*)hookLocation, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

    *hookLocation = 0xE9;
    *(DWORD*)(hookLocation + 1) = (DWORD)&GameloopCodecave - ((DWORD)hookLocation + 5);
    *(hookLocation + 5) = 0xC3; // ret

}

void CreateUnitHook()
{
    unsigned char* hookLocation = (unsigned char*)(base + HackValues::createUnitHookLocationOffset);
    recruitCreateFunctionAddr = base + HackValues::createUnitFunctionLocationOffset;
    recruitUnitRetAddress = (DWORD)hookLocation + 8;

    DWORD oldProtect;
    VirtualProtect((void*) hookLocation, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
    *hookLocation = 0xE9;
    *(DWORD*)(hookLocation + 1) = (DWORD)&RecruitUnitCodecave - ((DWORD)hookLocation + 5);
    *(hookLocation + 5) = 0x90;
    *(hookLocation + 6) = 0x90;
    *(hookLocation + 7) = 0x90;
}

void InitHooking()
{
    CreateUnitHook();
    CreateGameLoopHook();
}



BOOL WINAPI DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        base = (DWORD)GetModuleHandle(L"wyrmsun.exe");

        AllocConsole();
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);

        InitPointers();
        InitHooking();

        std::cout << *moneyPointer << std::endl;
        std::cout << *copperPointer << std::endl;
        std::cout << *foodPointer << std::endl;



        break;
    }
    return TRUE;
}

