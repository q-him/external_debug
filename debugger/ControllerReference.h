//
// Created by Q on 21.11.2022.
//

#ifndef EXTERNAL_DEBUG_CONTROLLERREFERENCE_H
#define EXTERNAL_DEBUG_CONTROLLERREFERENCE_H

#include <utility>
#include <iostream>
#include <boost/python.hpp>
#include "../shared/Controller.h"

class ControllerReference {
    std::shared_ptr<Controller> controller;
public:
    ControllerReference();
    explicit ControllerReference(std::shared_ptr<Controller> c);
    int get_param();
    void set_param(int param);
};

#endif //EXTERNAL_DEBUG_CONTROLLERREFERENCE_H
