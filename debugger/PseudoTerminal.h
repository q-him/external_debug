//
// Created by avkozyrev on 18.11.22.
//

#ifndef EXTERNAL_DEBUG_PSEUDOTERMINAL_H
#define EXTERNAL_DEBUG_PSEUDOTERMINAL_H

#include <array>
#include <string>
#include <pty.h>
#include <unistd.h>
#include <termios.h>
#include <stdexcept>
#include <boost/variant.hpp>
#include <vector>
#include <list>
#include <variant>
#include "InputHistory.h"

enum class ControlCode {
    BELL,
    BACKSPACE,
    TAB,
    LINE_FEED,
    FORM_FEED,
    CARRIAGE_RETURN,
    CURSOR_UP_1,
    CURSOR_DOWN_1,
    UNKNOWN,
    ERROR,
};

class PseudoTerminal {
public:
    PseudoTerminal();
    ~PseudoTerminal();
    void send(const std::string& s) const;
    void send(char c) const;
    std::string receive_line();
    std::string get_slave_name();
private:
    int master_handle = 0;
    int slave_handle = 0;
    std::array<char, 4097> buffer {};
    InputHistory history = InputHistory(20);

    [[nodiscard]] unsigned char receive_byte() const;
    [[nodiscard]] std::variant<ControlCode, unsigned char> read_code() const;
    [[nodiscard]] ControlCode read_escape_sequence() const;
    [[nodiscard]] ControlCode read_csi_sequence() const;
    std::string change_to_previous(const std::string& current);
    std::string change_to_next(const std::string &current);
    bool handle_control_code(ControlCode c, std::string& line);
    void erase_one_character(std::string& line);
    void send_erase_sequence(std::size_t count) const;
};


#endif //EXTERNAL_DEBUG_PSEUDOTERMINAL_H
