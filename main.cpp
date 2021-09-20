#include <iostream>
#include "client/linux/handler/exception_handler.h"

static bool dumpCallback(
    const google_breakpad::MinidumpDescriptor& descriptor,
    void *context,
    bool succeeded
) {
    std::cout << "Dump path: " << descriptor.path() << std::endl;
    return succeeded;    
}

int main(int, char**) {
    google_breakpad::MinidumpDescriptor descriptor(".");
    google_breakpad::ExceptionHandler handler(descriptor, nullptr, dumpCallback, nullptr, true, -1);

    // crash
    int *a = nullptr;
    *a = 1;
}
