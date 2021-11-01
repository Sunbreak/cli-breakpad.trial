#include <iostream>
#if defined _WIN32
#include "client/windows/handler/exception_handler.h"
#elif defined __APPLE__
#include "client/mac/handler/exception_handler.h"
#else
#include "client/linux/handler/exception_handler.h"
#endif

static bool dumpCallback(
#if defined _WIN32
    const wchar_t* dump_path,
    const wchar_t* minidump_id,
    void *context,
    EXCEPTION_POINTERS* exinfo,
    MDRawAssertionInfo* assertion,
#elif defined __APPLE__
    const char* dump_dir,
    const char* minidump_id,
    void *context,
#else
    const google_breakpad::MinidumpDescriptor& descriptor,
    void *context,
#endif
    bool succeeded
) {
#if defined _WIN32
    std::wcout << L"dump_path: " << dump_path << std::endl;
    std::wcout << L"minidump_id: " << minidump_id << std::endl;
#elif defined __APPLE__
    std::cout << "dump_dir: " << dump_dir << std::endl;
    std::cout << "minidump_id: " << minidump_id << std::endl;
#else
    std::cout << "Dump path: " << descriptor.path() << std::endl;
#endif
    return succeeded;    
}

int main(int, char**) {
#if defined _WIN32
    google_breakpad::ExceptionHandler handler(L".", nullptr, dumpCallback, nullptr, google_breakpad::ExceptionHandler::HANDLER_ALL);
#elif defined  __APPLE__
    google_breakpad::ExceptionHandler handler(".", nullptr, dumpCallback, nullptr, true, nullptr);
#else
    google_breakpad::MinidumpDescriptor descriptor(".");
    google_breakpad::ExceptionHandler handler(descriptor, nullptr, dumpCallback, nullptr, true, -1);
#endif

    // crash
    int *a = nullptr;
    *a = 1;
}
