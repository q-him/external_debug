//
// Created by avkozyrev on 17.11.22.
//

#ifndef EXTERNAL_DEBUG_DEBUGGER_H
#define EXTERNAL_DEBUG_DEBUGGER_H

#include "../shared/Controller.h"
#include <boost/python.hpp>
#include <iostream>

class ControllerReference {
    std::shared_ptr<Controller> controller;
public:
    ControllerReference();
    explicit ControllerReference(std::shared_ptr<Controller> c);
    int get_param();
    void set_param(int param);
};

extern "C" void start(std::shared_ptr<Controller> controller);

#endif //EXTERNAL_DEBUG_DEBUGGER_H