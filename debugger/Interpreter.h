//
// Created by avkozyrev on 18.11.22.
//

#ifndef EXTERNAL_DEBUG_INTERPRETER_H
#define EXTERNAL_DEBUG_INTERPRETER_H

#include <boost/python.hpp>

namespace python = boost::python;

class Interpreter {
public:
    Interpreter();
    void import(const std::string& module,  const std::string& name = "");
    std::string run(const std::string& code);
private:
    python::object main;
    python::dict locals;
    python::object globals;
    std::string exec(const std::string& code);
    std::string eval(const std::string& expression);
};

#endif //EXTERNAL_DEBUG_INTERPRETER_Ha
