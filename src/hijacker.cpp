#if defined(_WIN_PLATFORM_)

// bool TlsOnce = false;
// // this runs way before dllmain
// void __stdcall TlsCallback(PVOID hModule, DWORD fdwReason, PVOID pContext)
// {
//     if (!TlsOnce)
//     {
//         try
//         {
//             RunAsNode::Init();
//         }
//         catch (std::exception &e)
//         {
//             printf("Failed to Init RunAsNode: %s\n", e.what());
//         }
//         TlsOnce = true;
//     }
// }

// #pragma comment(linker, "/INCLUDE:_tls_used")
// #pragma comment(linker, "/INCLUDE:tls_callback_func")
// #pragma const_seg(".CRT$XLF")
// EXTERN_C const PIMAGE_TLS_CALLBACK tls_callback_func = TlsCallback;

// // version.dll DLLHijack
// extern "C" __declspec(dllexport) void GetFileVersionInfoA() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoByHandle() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoExA() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoExW() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoSizeA() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoSizeExA() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoSizeExW() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoSizeW() {}
// extern "C" __declspec(dllexport) void GetFileVersionInfoW() {}
// extern "C" __declspec(dllexport) void VerFindFileA() {}
// extern "C" __declspec(dllexport) void VerFindFileW() {}
// extern "C" __declspec(dllexport) void VerInstallFileA() {}
// extern "C" __declspec(dllexport) void VerInstallFileW() {}
// extern "C" __declspec(dllexport) void VerLanguageNameA() {}
// extern "C" __declspec(dllexport) void VerLanguageNameW() {}
// extern "C" __declspec(dllexport) void VerQueryValueA() {}
// extern "C" __declspec(dllexport) void VerQueryValueW() {}

// dbghelp.dll DLLHijack
extern "C" __declspec(dllexport) void StackWalk64() {}
extern "C" __declspec(dllexport) void SymCleanup() {}
extern "C" __declspec(dllexport) void SymFromAddr() {}
extern "C" __declspec(dllexport) void SymFunctionTableAccess64() {}
extern "C" __declspec(dllexport) void SymGetLineFromAddr64() {}
extern "C" __declspec(dllexport) void SymGetModuleBase64() {}
extern "C" __declspec(dllexport) void SymGetModuleInfo64() {}
extern "C" __declspec(dllexport) void SymGetSymFromAddr64() {}
extern "C" __declspec(dllexport) void SymGetSearchPathW() {}
extern "C" __declspec(dllexport) void SymInitialize() {}
extern "C" __declspec(dllexport) void SymSetOptions() {}
extern "C" __declspec(dllexport) void SymGetOptions() {}
extern "C" __declspec(dllexport) void SymSetSearchPathW() {}
extern "C" __declspec(dllexport) void UnDecorateSymbolName() {}
extern "C" __declspec(dllexport) void MiniDumpWriteDump() {}
#endif