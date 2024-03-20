// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <vector>
#include "hackoffsets.h"

using namespace std;

DWORD base;

DWORD* moneyPointer;
DWORD* incomePointer;


uintptr_t GetAddressFromOffsets(uintptr_t startAddress, vector<unsigned int> offsets)
{

    uintptr_t addr = startAddress;
    for (unsigned int i = 0; i < offsets.size(); i++)
    {
        addr = *(uintptr_t*)addr;
        addr += offsets[i];
    }

    return addr;

}

void WriteBytes(unsigned char* startLocation, vector<unsigned char> bytesToWrite)
{
    DWORD oldProtect;
    VirtualProtect((void*)startLocation, bytesToWrite.size(), PAGE_EXECUTE_READWRITE, &oldProtect);

    for (unsigned int i = 0; i < bytesToWrite.size(); i++)
    {
        *(startLocation + i) = bytesToWrite[i];
    }

    VirtualProtect((void*)startLocation, bytesToWrite.size(), oldProtect, &oldProtect);
}

void InitPointers()
{
    moneyPointer = (DWORD*)GetAddressFromOffsets(base + offsets::moneyBase, offsets::moneyOffsets);
    incomePointer = (DWORD*)GetAddressFromOffsets(base + offsets::moneyBase, offsets::incomeOffsets);
}

void InitMapHack()
{
    WriteBytes(offsets::mapHackLocation, offsets::mapHackBytes);
}

void InitNoFogHack()
{
    WriteBytes(offsets::noFogHackLocation, offsets::noFogHackBytes);
}

void InitHacks()
{
    InitPointers();
    //InitMapHack();
    InitNoFogHack();
}



BOOL WINAPI DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        base = (DWORD)GetModuleHandle(NULL);
        InitHacks();
        
        *moneyPointer = 1000;
        *incomePointer = 1000;

        break;
    }
    return TRUE;
}

