#include <iostream>
#ifdef __APPLE__
#include "client/mac/handler/exception_handler.h"
#else
#include "client/linux/handler/exception_handler.h"
#endif

static bool dumpCallback(
#ifdef __APPLE__
    const char* dump_dir,
    const char* minidump_id,
#else
    const google_breakpad::MinidumpDescriptor& descriptor,
#endif
    void *context,
    bool succeeded
) {
#ifdef __APPLE__
    std::cout << "dump_dir: " << dump_dir << std::endl;
    std::cout << "minidump_id: " << minidump_id << std::endl;
#else
    std::cout << "Dump path: " << descriptor.path() << std::endl;
#endif
    return succeeded;    
}

int main(int, char**) {
#ifdef __APPLE__
    google_breakpad::ExceptionHandler handler(".", nullptr, dumpCallback, nullptr, true, nullptr);
#else
    google_breakpad::MinidumpDescriptor descriptor(".");
    google_breakpad::ExceptionHandler handler(descriptor, nullptr, dumpCallback, nullptr, true, -1);
#endif

    // crash
    int *a = nullptr;
    *a = 1;
}
