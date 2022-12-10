#pragma once

struct GameMainThread
{
    static GameMainThread& Get();

    void AddBaseInitializationTask(const std::function<bool()>& aFunction);
    void AddInitializationTask(const std::function<bool()>& aFunction);
    void AddRunningTask(const std::function<bool()>& aFunction);
    void AddShutdownTask(const std::function<bool()>& aFunction);
    void AddGenericTask(const std::function<bool()>& aFunction);

private:
    GameMainThread() = default;

    using TStateTick = bool();

    static bool HookStateTick();

    // helper task queue which executes added tasks each drain until they are finished
    struct RepeatedTaskQueue
    {
        void AddTask(const std::function<bool()>& aFunction);
        void Drain();

    private:
        std::recursive_mutex m_mutex;
        TiltedPhoques::Vector<std::function<bool()>> m_tasks;
    };

    struct StateTickOverride
    {
        StateTickOverride(const uintptr_t acOffset, const char* acpRealFunctionName);
        ~StateTickOverride();

        bool OnTick();

        uint8_t* Location = nullptr;
        TStateTick* RealFunction = nullptr;
        RepeatedTaskQueue Tasks;
    };

    std::array<StateTickOverride, 0> m_stateTickOverrides{};

    RepeatedTaskQueue m_genericQueue;
};
