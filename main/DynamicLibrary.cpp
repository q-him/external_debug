//
// Created by avkozyrev on 18.11.22.
//

#include "DynamicLibrary.h"

DynamicLibrary::DynamicLibrary(const std::string &path) {
    handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        throw std::runtime_error(dlerror());
    }
}

DynamicLibrary::~DynamicLibrary() {
    if (handle) {
        dlclose(handle);
    }
}
