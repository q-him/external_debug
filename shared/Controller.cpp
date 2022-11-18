//
// Created by avkozyrev on 17.11.22.
//

#include <iostream>
#include "Controller.h"

int Controller::get_param() {
    const std::lock_guard<std::mutex> lock(mtx);
    return param;
}

void Controller::set_param(int new_param) {
    const std::lock_guard<std::mutex> lock(mtx);
    param = new_param;
}

Controller::Controller(const Controller &c) : param(c.param) {
    std::cout << "Copy constructor" << std::endl;
}
