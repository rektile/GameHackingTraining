// dllmain.cpp : Defines the entry point for the DLL application.
// For Flare x86
#include <Windows.h>
#include <vector>
#include "HackValues.h"
#include <iostream>

DWORD moduleBase;

float* playerX;
float* playerY;
float* enemyX;
float* enemyY;
DWORD* mouseX;
DWORD* mouseY;

DWORD enemyLocHookRetAddr;

INPUT input = { 0 };

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

void InitPointers()
{
    playerX = (float*)GetAddressFromOffsets(moduleBase + HackValues::locationBase, HackValues::locationXOffsets);
    playerY = (float*)GetAddressFromOffsets(moduleBase + HackValues::locationBase, HackValues::locationYOffsets);
    mouseX = (DWORD*)GetAddressFromOffsets(moduleBase + HackValues::mouseBase, HackValues::mouseXOffsets);
    mouseY = (DWORD*)GetAddressFromOffsets(moduleBase + HackValues::mouseBase, HackValues::mouseYOffsets);
}

__declspec(naked) void enemyLocCodeCave()
{
    __asm
    {
        pushad
        lea eax, [ecx+0x240]
        mov enemyX, eax
        lea eax, [ecx+0x244]
        mov enemyY, eax
        popad
        fld dword ptr ds:[ecx + 0x244]
        jmp enemyLocHookRetAddr
    }
}

void CreateEnemyLocHook()
{
    unsigned char* hookLocation = (unsigned char*)(moduleBase + HackValues::enemyMovementHookLocationOffset);
    enemyLocHookRetAddr = (DWORD)hookLocation + 5;

    DWORD oldProtect;
    VirtualProtect((void*)hookLocation, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
    *hookLocation = 0xE9;
    *(DWORD*)(hookLocation + 1) = (DWORD)&enemyLocCodeCave - ((DWORD)hookLocation + 5);
    *(hookLocation + 5) = 0x90;
}

void InitHooks()
{
    CreateEnemyLocHook();
}

void InjectedThread()
{


    while (true)
    {
        bool doesPlayerExist = playerX != NULL && playerY != NULL;
        bool doesEnemyExist = enemyX != NULL && enemyY != NULL;
        bool playerAndEnemyNotSameSpot = playerX != enemyX && playerY != enemyY;
        bool haveMouse = mouseX != NULL && mouseY != NULL;

        if (doesPlayerExist && doesEnemyExist && playerAndEnemyNotSameSpot && haveMouse && GetAsyncKeyState('M'))
        {
            if (*enemyX < *playerX) {
                *mouseX = 490;
            }
            else {
                *mouseX = 560;
            }

            if (*enemyY < *playerY) {
                *mouseY = 270;
            }
            else {
                *mouseY = 330;
            }

            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }

        Sleep(1);
        
    }

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        moduleBase = (DWORD)GetModuleHandle(L"flare.exe");

        AllocConsole();
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);

        InitPointers();
        InitHooks();

        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InjectedThread, NULL, 0, NULL);

        break;
    }
    return TRUE;
}

