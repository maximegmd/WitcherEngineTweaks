#include <stdafx.h>

#include "LuaVM.h"

#include <WET.h>
#include <Utils.h>

namespace
{

LuaVM* s_vm{nullptr};

using TOodleLZ_Decompress =
    size_t (*)(char* in, int insz, char* out, int outsz, int wantsFuzzSafety, int b, int c, void* d, void* e, void* f, void* g, void* workBuffer, size_t workBufferSize, int j);

struct Header
{
    uint32_t m_magic = 0;   // a hash of all types currently supported
    uint32_t m_version = 0; // 1
    uint32_t m_recordsCount = 0;
    uint32_t m_flatsCount = 0;
    uint32_t m_queriesCount = 0;
};

int32_t ReadCompressedInt(std::istream& aFile)
{
    int32_t uncompressed = 0;

    int8_t byte;
    aFile.read(reinterpret_cast<char*>(&byte), 1);
    uncompressed |= byte & 0x3F;
    if (byte & 0x40)
    {
        aFile.read(reinterpret_cast<char*>(&byte), 1);
        uncompressed |= (byte & 0x7F) << 6;
        if (byte & 0x80)
        {
            aFile.read(reinterpret_cast<char*>(&byte), 1);
            uncompressed |= (byte & 0x7F) << 13;
            if (byte & 0x80)
            {
                aFile.read(reinterpret_cast<char*>(&byte), 1);
                uncompressed |= (byte & 0x7F) << 20;
                if (byte & 0x80)
                {
                    aFile.read(reinterpret_cast<char*>(&byte), 1);
                    uncompressed |= byte << 27;
                }
            }
        }
    }

    return uncompressed;
}
} // namespace

LuaVM::LuaVM(const Paths& aPaths, VKBindings& aBindings, D3D12& aD3D12)
    : m_scripting(aPaths, aBindings, aD3D12)
    , m_d3d12(aD3D12)
{
    Hook();

    aBindings.SetVM(this);
}

void LuaVM::Hook()
{
    s_vm = this;

    GameMainThread::Get().AddRunningTask(
        [this]
        {
            const auto cNow = std::chrono::high_resolution_clock::now();
            const auto cDelta = cNow - s_vm->m_lastframe;
            const auto cSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(cDelta);

            s_vm->Update(cSeconds.count());

            s_vm->m_lastframe = cNow;

            return false;
        });

    GameMainThread::Get().AddShutdownTask(
        [this]
        {
            s_vm->m_scripting.UnloadAllMods();

            return true;
        });
}
