//
// Created by avkozyrev on 17.11.22.
//

#include "debugger.h"

#include <utility>

namespace python = boost::python;

BOOST_PYTHON_MODULE(debugger)
{
    python::class_<ControllerReference>("Controller")
            .add_property("param", &ControllerReference::get_param, &ControllerReference::set_param)
    ;
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

    try
    {
        global["debugger"] = python::import("debugger");
        global["c"] = ControllerReference(std::move(controller));
        python::exec("c.param = 200", global);
        python::exec("print(c.param)", global);
        python::exec("print(a)", global, local);
        python::exec("a = 2", global, local);
        python::exec("print(a)", global, local);
    }
    catch (python::error_already_set const &)
    {
        PyErr_Print();
    }
}

ControllerReference::ControllerReference(std::shared_ptr<Controller> c) : controller(std::move(c)) {
}

int ControllerReference::get_param() {
    return controller->get_param();
}

void ControllerReference::set_param(int param) {
    controller->set_param(param);
}

ControllerReference::ControllerReference() : controller(std::make_shared<Controller>()){

}
