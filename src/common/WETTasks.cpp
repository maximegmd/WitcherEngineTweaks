#include <stdafx.h>

#include "WETTasks.h"

WETTasks::WETTasks()
    : m_running(true)
{
}

WETTasks::~WETTasks()
{
    m_running.store(false);
}
