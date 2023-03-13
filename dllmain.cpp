#include <iostream>
#include <windows.h>
#include "pch.h"

using namespace std;
DWORD addrRet = 0x004025ED;

__declspec(naked) void hookThing() {
    __asm {
        mov eax, 420
    }
    __asm {
        jmp addrRet
    }
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved) {
    DWORD old_protect;
    unsigned char* hookLoc = (unsigned char*)0x004025E8;
    if (dwReason == DLL_PROCESS_ATTACH) {
        VirtualProtect((void*)hookLoc, 5, PAGE_EXECUTE_READWRITE, &old_protect);
        *hookLoc = 0xE9; //jmp
        *(DWORD*)(hookLoc + 1) = (DWORD)&hookThing - ((DWORD)hookLoc + 5);
    }
    return TRUE;
}

