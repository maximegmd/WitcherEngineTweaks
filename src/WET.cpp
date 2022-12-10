#include <stdafx.h>

#include "WET.h"
#include "Options.h"

using namespace std::chrono_literals;

static std::unique_ptr<WET> s_pInstance{nullptr};
static bool s_isRunning{true};

void WET::Initialize()
{
    s_pInstance.reset(new WET);
}

void WET::Shutdown()
{
    s_pInstance.reset(nullptr);
}

WET& WET::Get()
{
    // we should always call this after initialization, never before!
    assert(s_pInstance);
    return *s_pInstance;
}

const Paths& WET::GetPaths() const noexcept
{
    return m_paths;
}

const Options& WET::GetOptions() const noexcept
{
    return m_options;
}

const PersistentState& WET::GetPersistentState() const noexcept
{
    return m_persistentState;
}

D3D12& WET::GetD3D12() noexcept
{
    return m_d3d12;
}

VKBindings& WET::GetBindings() noexcept
{
    return m_bindings;
}

Overlay& WET::GetOverlay() noexcept
{
    return m_overlay;
}

LuaVM& WET::GetVM() noexcept
{
    return m_vm;
}

bool WET::IsRunning() noexcept
{
    return s_isRunning;
}

WET::WET()
    : m_options(m_paths)
    , m_persistentState(m_paths, m_options)
    , m_bindings(m_paths, m_options)
    , m_window(&m_bindings, &m_d3d12)
    , m_d3d12(m_window, m_paths, m_options)
    , m_vm(m_paths, m_bindings, m_d3d12)
    , m_overlay(m_bindings, m_options, m_persistentState, m_vm)
{
    m_vm.Initialize();
}

WET::~WET()
{
    s_isRunning = false;
}
