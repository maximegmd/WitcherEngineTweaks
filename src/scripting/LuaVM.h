#pragma once

#include "Scripting.h"

struct UnknownString;

struct TDBIDLookupEntry
{
    uint64_t base;
    std::string name;
};

struct Image;
struct LuaVM
{
    LuaVM(const Paths& aPaths, VKBindings& aBindings, D3D12& aD3D12);
    ~LuaVM() = default;

    [[nodiscard]] const VKBind* GetBind(const VKModBind& acModBind) const;
    [[nodiscard]] const TiltedPhoques::Vector<VKBind>* GetBinds(const std::string& acModName) const;
    [[nodiscard]] const TiltedPhoques::Map<std::string, std::reference_wrapper<const TiltedPhoques::Vector<VKBind>>>& GetAllBinds() const;

    bool ExecuteLua(const std::string& acCommand) const;

    void Update(float aDeltaTime);
    void Draw() const;
    void ReloadAllMods();

    void OnOverlayOpen() const;
    void OnOverlayClose() const;

    void Initialize();

    bool IsInitialized() const;

    void BlockDraw(bool aBlockDraw);

    void PostInitializeScripting();
    void PostInitializeMods();

protected:
    void Hook();

private:
    Scripting m_scripting;

    bool m_initialized{false};
    bool m_drawBlocked{false};
    bool m_reload{false};

    D3D12& m_d3d12;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastframe{std::chrono::high_resolution_clock::now()};
};
