#if defined(_WIN_PLATFORM_)
#include "run_as_node.h"

#include <vector>
#include <algorithm>
#include <codecvt>
#include <map>

#if defined(_WIN_PLATFORM_)
#define CURRENT_VERSION "9.9.12-25234"
#if defined(_X64_ARCH_) // {call winmain, check run as node function}
std::map<std::string, std::pair<uint64_t, uint64_t>> mainAddrMap = {
    {"9.9.12-25234", {0x457A76D, 0x3A5D70}}};
#endif
#endif

int(__stdcall *oriWinMain)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

void(__fastcall *checkRunAsNode)(void *a1);

std::shared_ptr<void> (*nodeInitializeOncePerProcess)(
    const std::vector<std::string> &args,
    uint32_t flags);

int __stdcall fakeWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    MessageBoxA(NULL, "Init", "fakeWinMain", MB_OK);
    checkRunAsNode(nullptr);
    return oriWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

bool RunAsNode::RunNode()
{
    struct Arguments
    {
        int argc = 0;
        wchar_t **argv =
            ::CommandLineToArgvW(::GetCommandLineW(), &argc);

        ~Arguments() { LocalFree(argv); }
    } arguments;

    std::vector<std::string> argv(arguments.argc);

    std::transform(arguments.argv, arguments.argv + arguments.argc, argv.begin(),
                   [](auto &a)
                   { return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(a); });

    MessageBoxA(NULL, "pre nodeInitializeOncePerProcess", "fakeWinMain", MB_OK);
    std::for_each(argv.begin(), argv.end(), [](const std::string &arg)
                  { MessageBoxA(NULL, arg.c_str(), "fakeWinMain", MB_OK);});
    nodeInitializeOncePerProcess(argv, (1 << 6) | (1 << 7));
    MessageBoxA(NULL, "post nodeInitializeOncePerProcess", "fakeWinMain", MB_OK);
    return true;
}

bool RunAsNode::Init()
{
    uint64_t baseAddr = 0;
#if defined(_WIN_PLATFORM_)
    HMODULE wrapperModule = GetModuleHandleW(NULL);
    if (wrapperModule == NULL)
        throw std::runtime_error("Can't GetModuleHandle");
    baseAddr = reinterpret_cast<uint64_t>(wrapperModule);
    printf("baseAddr: %llx\n", baseAddr);
#elif defined(_MAC_PLATFORM_)
    auto pmap = hak::get_maps();
    do
    {
        if (pmap->module_name.find("QQ") != std::string::npos && pmap->offset == 0)
        {
            baseAddr = pmap->start();
            printf("baseAddr: %llx\n", baseAddr);
            break;
        }
    } while ((pmap = pmap->next()) != nullptr);
#elif defined(_LINUX_PLATFORM_)
    auto pmap = hak::get_maps();
    do
    {
        if (pmap->module_name.find("QQ") != std::string::npos && pmap->offset == 0)
        {
            baseAddr = pmap->start();
            printf("baseAddr: %lx\n", baseAddr);
            break;
        }
    } while ((pmap = pmap->next()) != nullptr);
#endif
    if (baseAddr == 0)
        throw std::runtime_error("Can't find hook address");

    auto [callptr, funcptr] = mainAddrMap[CURRENT_VERSION];

    uint8_t *abscallptr = reinterpret_cast<uint8_t *>(baseAddr + callptr);
    oriWinMain = reinterpret_cast<int(__stdcall *)(HINSTANCE, HINSTANCE, LPSTR, int)>(moehoo::get_call_address(abscallptr));
    checkRunAsNode = reinterpret_cast<void(__fastcall *)(void *)>(baseAddr + funcptr);
    nodeInitializeOncePerProcess = reinterpret_cast<std::shared_ptr<void> (*)(const std::vector<std::string> &, uint32_t)>(baseAddr + 0x1FFF710);
    return moehoo::hook(abscallptr, &fakeWinMain);
}

#endif