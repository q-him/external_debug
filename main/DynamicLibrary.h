//
// Created by avkozyrev on 18.11.22.
//

#ifndef EXTERNAL_DEBUG_DYNAMICLIBRARY_H
#define EXTERNAL_DEBUG_DYNAMICLIBRARY_H

#include "../shared/hash.h"
#include <dlfcn.h>
#include <functional>
#include <string>
#include <stdexcept>

class DynamicLibrary {
    void* handle;
public:
    explicit DynamicLibrary(const std::string& path);
    ~DynamicLibrary();

    template<typename F>
    std::function<F> get_function(const std::string &name) const {
        auto raw_function = dlsym(handle, name.c_str());

        if (raw_function == nullptr) {
            throw std::runtime_error(dlerror());
        }

        return reinterpret_cast<F*>(raw_function);
    }
};

#endif //EXTERNAL_DEBUG_DYNAMICLIBRARY_H
