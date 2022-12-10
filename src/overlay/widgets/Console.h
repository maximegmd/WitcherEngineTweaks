#pragma once

#include "Widget.h"
#include "LogWindow.h"

struct LuaVM;
struct Console : Widget
{
    Console(Options& aOptions, PersistentState& aPersistentState, LuaVM& aVm);
    ~Console() override = default;

    WidgetResult OnDisable() override;

protected:
    void OnUpdate() override;

private:
    static int HandleConsoleHistory(ImGuiInputTextCallbackData* apData);
    static int HandleConsoleResize(ImGuiInputTextCallbackData* apData);
    static int HandleConsole(ImGuiInputTextCallbackData* apData);

    Options& m_options;
    PersistentState& m_persistentState;
    LuaVM& m_vm;

    LogWindow m_logWindow;

    size_t m_historyIndex{0};
    bool m_newHistory{true};

    std::string m_command;
    int m_commandLength{0};
};
