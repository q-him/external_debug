//
// Created by avkozyrev on 18.11.22.
//

#include <iostream>
#include "PseudoTerminal.h"

PseudoTerminal::PseudoTerminal() {
    struct termios tty {};
    tty.c_iflag = (tcflag_t) 0;
    tty.c_lflag = (tcflag_t) 0;
    tty.c_cflag = CS8;
    tty.c_oflag = (tcflag_t) 0;

    int res = openpty(&master_handle, &slave_handle, nullptr, &tty, nullptr);
    if (res < 0) {
        throw std::runtime_error("Cannot open PTY");
    }
}

void PseudoTerminal::send(const std::string &s) const {
    write(master_handle, s.c_str(), s.length());
}

PseudoTerminal::~PseudoTerminal() {
    close(slave_handle);
    close(master_handle);
}

std::string PseudoTerminal::get_slave_name() {
    int res = ttyname_r(slave_handle, buffer.data(), buffer.size());

    if (res != 0) {
        return "error";
    } else {
        return {buffer.data()};
    }
}

std::string PseudoTerminal::read_line() const {
    std::string line;
    ssize_t count = 0;
    char c;
    do {
        count = read(master_handle, &c, 1);
        if (count < 0) {
            throw std::runtime_error("Error reading from PseudoTerminal");
        }

        line += c;
        write(master_handle, &c, 1);
//        std::cout << "Received char code: " << std::hex << static_cast<int>(c) << std::dec << std::endl;
    } while (count > 0 && c != '\n' && c != '\r');
    write(master_handle, "\r\n", 2);

    return line;
}