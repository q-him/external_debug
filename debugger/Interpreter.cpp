//
// Created by avkozyrev on 18.11.22.
//

#include "Interpreter.h"

Interpreter::Interpreter() {
    main = python::import("__main__");
    globals = main.attr("__dict__");
}

void Interpreter::import(const std::string& module, const std::string& name) {
    auto imported = python::import(module.c_str());

    if (name.empty()) {
        globals[module] = imported;
    } else {
        globals[name] = imported;
    }
}