//
// Created by avkozyrev on 18.11.22.
//

#include "DynamicLibrary.h"

DynamicLibrary::DynamicLibrary(const std::string &path) {
    handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        throw std::runtime_error(dlerror());
    }

    auto get_shared_hash = get_function<std::string(void)>("get_shared_hash");
    if (get_shared_hash() != SHARED_HASH) {
        throw std::runtime_error("Cannot load dynamic library: version mismatch");
    }
}

DynamicLibrary::~DynamicLibrary() {
    if (handle) {
        dlclose(handle);
    }
}
