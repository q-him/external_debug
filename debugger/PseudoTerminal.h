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


class PseudoTerminal {
    int master_handle = 0;
    int slave_handle = 0;
    std::array<char, 4097> buffer {};
public:
    PseudoTerminal();
    ~PseudoTerminal();
    void send(const std::string& s) const;
    std::string read_line();
    std::string get_slave_name();
};


#endif //EXTERNAL_DEBUG_PSEUDOTERMINAL_H
