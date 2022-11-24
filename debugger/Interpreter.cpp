//
// Created by avkozyrev on 18.11.22.
//

#include "Interpreter.h"

Interpreter::Interpreter() {
    main = python::import("__main__");
    globals = main.attr("__dict__");
}

void Interpreter::import(const std::string &module, const std::string &name) {
    auto imported = python::import(module.c_str());

    if (name.empty()) {
        globals[module] = imported;
    } else {
        globals[name] = imported;
    }
}

std::string handle_pyerror() {
    PyObject * exc, *val, *tb;
    python::object formatted_list, formatted;
    PyErr_Fetch(&exc, &val, &tb);
    PyErr_NormalizeException(&exc, &val, &tb);
    python::handle<> hexc(exc), hval(python::allow_null(val)), htb(python::allow_null(tb));
    python::object traceback(python::import("traceback"));
    if (!tb) {
        python::object format_exception_only(traceback.attr("format_exception_only"));
        formatted_list = format_exception_only(hexc, hval);
    } else {
        python::object format_exception(traceback.attr("format_exception"));
        formatted_list = format_exception(hexc, hval, htb);
    }
    formatted = python::str().join(formatted_list);
    return python::extract<std::string>(formatted); // NOLINT(modernize-return-braced-init-list)
}

std::string Interpreter::eval(const std::string &expression) {
    auto result = python::eval(expression.c_str(), globals, locals);
    auto as_str = python::str(result);
    std::string as_string = python::extract<std::string>(as_str); // NOLINT(modernize-return-braced-init-list)
    return as_string;
}

std::string Interpreter::exec(const std::string &code) {
    try {
        python::exec(code.c_str(), globals, locals);
        return "";
    }
    catch (python::error_already_set const &) {
        std::string message;
        if (PyErr_Occurred()) {
            message = handle_pyerror();
        }
        python::handle_exception();
        PyErr_Clear();
        return message;
    }
}

std::string Interpreter::run(const std::string &code) {
    try {
        return eval(code);
    }
    catch (python::error_already_set const &) {
        PyErr_Clear();
        return exec(code);
    }
}
