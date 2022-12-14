//
// Created by avkozyrev on 17.11.22.
//

#include "debugger.h"
#include "ControllerReference.h"
#include "PseudoTerminal.h"
#include "Interpreter.h"

#include <thread>
#include <utility>
#include <boost/algorithm/string.hpp>

namespace python = boost::python;

static std::thread debug_thread;

BOOST_PYTHON_MODULE(debugger)
{
    python::class_<ControllerReference>("Controller")
            .add_property("param", &ControllerReference::get_param, &ControllerReference::set_param)
    ;
}

void debugger_loop(PseudoTerminal& pt, Interpreter& interpreter) {
    pt.send(">>> ");

    std::string input = pt.receive_line();
    std::string result = interpreter.run(input);

    if (!result.empty()) {
        boost::replace_all(result, "\n", "\r\n");
        boost::trim_right(result);
        pt.send(result);
        pt.send("\r\n");
    }
}

[[noreturn]] void debug_thread_body() {
    PseudoTerminal pt;
    std::cout << "Debugger device: " << pt.get_slave_name() << std::endl;

    Interpreter interpreter;

    while (true) {
        debugger_loop(pt, interpreter);
    }
}

extern "C" void start(std::shared_ptr<Controller> controller) {
    std::cout << "libdebugger loaded" << std::endl;

    if (PyImport_AppendInittab("debugger", PyInit_debugger) == -1)
        throw std::runtime_error("Failed to add debugger to the interpreter's "
                                 "builtin modules");

    Py_Initialize();

    auto main = python::import("__main__");
    auto global = main.attr("__dict__");
    auto local = python::dict();
    local["a"] = 1;

    global["debugger"] = python::import("debugger");
    global["c"] = ControllerReference(std::move(controller));

    debug_thread = std::thread(debug_thread_body);
}

[[maybe_unused]] std::string get_shared_hash() {
    return SHARED_HASH;
}
