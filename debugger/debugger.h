//
// Created by avkozyrev on 17.11.22.
//

#ifndef EXTERNAL_DEBUG_DEBUGGER_H
#define EXTERNAL_DEBUG_DEBUGGER_H

#include "../shared/Controller.h"
#include "../shared/hash.h"
#include <boost/python.hpp>
#include <iostream>

extern "C" [[maybe_unused]] std::string get_shared_hash();
extern "C" void start(std::shared_ptr<Controller> controller);

#endif //EXTERNAL_DEBUG_DEBUGGER_H
