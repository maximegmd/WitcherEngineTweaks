#pragma once

struct WETTasks
{
    WETTasks();
    ~WETTasks();

private:
    std::atomic<bool> m_running;
};