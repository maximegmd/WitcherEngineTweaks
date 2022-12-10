#include <stdafx.h>

#include "WET.h"
#include "Options.h"
#include "scripting/GameHooks.h"

static HANDLE s_modInstanceMutex = nullptr;

using namespace std::chrono_literals;

static void Initialize()
{
    try
    {
        MH_Initialize();

        WET::Initialize();

        const auto& options = WET::Get().GetOptions();

        // single instance check
        s_modInstanceMutex = CreateMutex(nullptr, TRUE, _T("Witcher Engine Tweaks Module Instance"));
        if (s_modInstanceMutex == nullptr)
            return;

        MH_EnableHook(nullptr);
    }
    catch (...)
    {
    }
}

static void Shutdown()
{
    bool inGameProcess = false;

    if (s_modInstanceMutex)
    {
        inGameProcess = WET::Get().GetOptions().ExeValid;

        MH_DisableHook(nullptr);
        MH_Uninitialize();

        WET::Shutdown();

        ReleaseMutex(s_modInstanceMutex);
    }

    if (inGameProcess)
    {
        // flush main log (== default logger)
        spdlog::default_logger()->flush();
        spdlog::get("scripting")->flush();
    }
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD ul_reason_for_call, LPVOID)
{
    DisableThreadLibraryCalls(mod);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: Initialize(); break;
    case DLL_PROCESS_DETACH: Shutdown(); break;
    default: break;
    }

    return TRUE;
}
