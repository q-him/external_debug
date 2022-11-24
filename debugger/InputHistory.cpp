//
// Created by avkozyrev on 24.11.22.
//

#include "InputHistory.h"

InputHistory::InputHistory(std::size_t max_size) : max_size(max_size) {

}

void InputHistory::add(const std::string& input) {
    reset_index();

    if (history.size() >= max_size) {
        history.pop_front();
    }

    history.push_back(input);
}

std::string InputHistory::previous() {
    if (history.empty()) {
        return "";
    }

    if (index == 0) {
        index = history.size() - 1;
    } else {
        index--;
    }

    return history[index];
}

std::string InputHistory::next() {
    if (history.empty() || index == 0) {
        return "";
    }

    index = (index + 1) % history.size();
    return history[index];
}

void InputHistory::reset_index() {
    index = 0;
}
