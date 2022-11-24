//
// Created by avkozyrev on 24.11.22.
//

#ifndef EXTERNAL_DEBUG_INPUTHISTORY_H
#define EXTERNAL_DEBUG_INPUTHISTORY_H

#include <deque>
#include <string>

class InputHistory {
public:
    explicit InputHistory(std::size_t max_size);
    void add(const std::string& input);
    std::string previous();
    std::string next();
    void reset_index();
private:
    std::size_t max_size;
    std::deque<std::string> history;
    std::size_t index = 0;
};


#endif //EXTERNAL_DEBUG_INPUTHISTORY_H
