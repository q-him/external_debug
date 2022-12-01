#include "../shared/Controller.h"
#include "../shared/hash.h"
#include "DynamicLibrary.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

using namespace std::chrono_literals;

int main() {
    std::cout << SHARED_HASH << std::endl;

    auto controller = std::make_shared<Controller>();

    DynamicLibrary lib("./libdebugger.so");
    std::cout << "lib loaded" << std::endl;

    auto start_debugger = lib.get_function<void(std::shared_ptr<Controller>)>("start");
    std::cout << "function found" << std::endl;

    start_debugger(controller);


    while (true) {
        std::cout << "Param = " << controller->get_param() << std::endl;

        if (controller->get_param() < 0) {
            return 0;
        }

        std::this_thread::sleep_for(2000ms);
    }
}
