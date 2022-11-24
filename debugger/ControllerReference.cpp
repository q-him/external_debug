//
// Created by Q on 21.11.2022.
//

#include "ControllerReference.h"

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