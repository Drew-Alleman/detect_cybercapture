// detect-cybercapture-stealth.cpp
// Build: cl /EHsc /O2 /std:c++17 detect-cybercapture-stealth.cpp /link psapi.lib
// Exit code: 1 = CyberCapture present, 0 = clean

#include <cstring>
#include <Windows.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

static void BuildInstall(char* o)
{
    o[0] = 'S'; o[1] = 'n'; o[2] = 'x'; o[3] = 'H';
    o[4] = 'k'; o[5] = '_'; o[6] = 'I'; o[7] = 'n';
    o[8] = 's'; o[9] = 't'; o[10] = 'a'; o[11] = 'l';
    o[12] = 'l'; o[13] = 'H'; o[14] = 'o'; o[15] = 'o';
    o[16] = 'k'; o[17] = 0;
}

static bool IsGoodRegion(uintptr_t addr)
{
    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)))
        return false;

    // Only the region types observed in CyberCapture
    if (!(mbi.Type & (MEM_MAPPED | MEM_IMAGE)))
        return false;

    if (mbi.State != MEM_COMMIT)
        return false;
    if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
        return false;

    const DWORD readable = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
        PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
    return (mbi.Protect & readable) != 0;
}

static bool ScanForCyberCapture(uintptr_t selfBase, size_t selfSize,
    uintptr_t exclStart, uintptr_t exclEnd,
    const char* needle, size_t len)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    unsigned char* addr = static_cast<unsigned char*>(si.lpMinimumApplicationAddress);
    unsigned char* max = static_cast<unsigned char*>(si.lpMaximumApplicationAddress);
    MEMORY_BASIC_INFORMATION mbi;

    while (addr < max && VirtualQuery(addr, &mbi, sizeof(mbi)) == sizeof(mbi))
    {
        // Skip our own image
        uintptr_t base = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
        uintptr_t end = base + mbi.RegionSize;
        if (base < selfBase + selfSize && end > selfBase)
        {
            addr = reinterpret_cast<unsigned char*>(end);
            continue;
        }

        if ((mbi.State == MEM_COMMIT) &&
            (mbi.Type & (MEM_MAPPED | MEM_IMAGE | MEM_PRIVATE)) &&
            !(mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) &&
            (mbi.Protect != PAGE_EXECUTE))
        {
            const unsigned char* p = static_cast<const unsigned char*>(mbi.BaseAddress);
            const unsigned char* pend = p + mbi.RegionSize;

            __try
            {
                while (p + len <= pend)
                {
                    if (*p == needle[0])
                    {
                        uintptr_t hit = reinterpret_cast<uintptr_t>(p);

                        // Skip live needle buffer
                        if (hit >= exclStart && hit < exclEnd)
                        {
                            ++p;
                            continue;
                        }

                        if (memcmp(p, needle, len) == 0 && IsGoodRegion(hit))
                            return true;   // early exit
                    }
                    ++p;
                }
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {}
        }

        unsigned char* next = static_cast<unsigned char*>(mbi.BaseAddress) + mbi.RegionSize;
        if (next <= addr) break;
        addr = next;
    }
    return false;
}

int main()
{
    char needle[24]{};
    BuildInstall(needle);

    // Live buffer to ignore
    uintptr_t exclStart = reinterpret_cast<uintptr_t>(needle);
    uintptr_t exclEnd = exclStart + sizeof(needle);

    // Own image
    HMODULE hSelf = GetModuleHandleW(nullptr);
    MODULEINFO mi{};
    GetModuleInformation(GetCurrentProcess(), hSelf, &mi, sizeof(mi));

    bool detected = ScanForCyberCapture(
        reinterpret_cast<uintptr_t>(mi.lpBaseOfDll), mi.SizeOfImage,
        exclStart, exclEnd,
        needle, 17);

    if (detected)
    {
        MessageBoxA(nullptr,
            "CyberCapture detected\n(SnxHk_InstallHook in MAPPED/IMAGE region)",
            "Detection", MB_OK | MB_ICONWARNING);
    }
    else
    {
        MessageBoxA(nullptr,
            "No CyberCapture indicators found",
            "Detection", MB_OK | MB_ICONINFORMATION);
    }

    return detected ? 1 : 0;
}