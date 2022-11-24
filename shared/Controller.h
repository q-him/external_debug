//
// Created by avkozyrev on 17.11.22.
//

#ifndef EXTERNAL_DEBUG_CONTROLLER_H
#define EXTERNAL_DEBUG_CONTROLLER_H

#include <mutex>

class Controller {
    std::mutex mtx;
    int param = 200;
public:
    Controller() = default;
    Controller(const Controller &c);

    int get_param();
    void set_param(int new_param);
};

#endif //EXTERNAL_DEBUG_CONTROLLER_H
