#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>

uint64_t originalSwapBuffers;
std::once_flag flag1;

int swapBuffersCallback(_In_ HDC hdc) {
    std::call_once(flag1, [&](){
        std::cout << "hooked" << std::endl;
    });
    return PLH::FnCast(originalSwapBuffers, &swapBuffersCallback)(hdc);
}

int entry() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    const auto swapBuffers = GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    std::cout << "SwapBuffers address: " << std::hex << swapBuffers << std::endl;
    PLH::CapstoneDisassembler dis(PLH::Mode::x64);
    PLH::x64Detour detour((char*)swapBuffers, (char*)&swapBuffersCallback, &originalSwapBuffers, dis);
    detour.hook();
    return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved) {
    if(callReason != DLL_PROCESS_ATTACH)
        return FALSE;

    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)entry, hModule, 0, nullptr);
    return TRUE;
}