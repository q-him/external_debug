//
// Created by avkozyrev on 18.11.22.
//

#include <iostream>
#include "PseudoTerminal.h"
#include "EndOfFileError.h"

PseudoTerminal::PseudoTerminal() {
    struct termios tty{};
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

void PseudoTerminal::send(char c) const {
    write(master_handle, &c, 1);
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

std::string PseudoTerminal::receive_line() {
    std::string line;
    std::variant<ControlCode, unsigned char> code;
    do {
        code = read_code();

        if (std::holds_alternative<unsigned char>(code)) {
            char c = static_cast<char>(std::get<unsigned char>(code));
            line += c;
            send(c);
        } else {
            ControlCode c = std::get<ControlCode>(code);
            if (!PseudoTerminal::handle_control_code(c, line)) {
                break;
            }
        }
    } while (true);
    write(master_handle, "\r\n", 2);

    if (!line.empty()) {
        history.add(line);
    }

    return line;
}

bool PseudoTerminal::handle_control_code(ControlCode c, std::string &line) {
    switch (c) {
        case ControlCode::CARRIAGE_RETURN:
        case ControlCode::LINE_FEED:
            return false;
        case ControlCode::CURSOR_UP_1:
            line = change_to_previous(line);
            return true;
        case ControlCode::CURSOR_DOWN_1:
            line = change_to_next(line);
            return true;
        case ControlCode::BACKSPACE:
            erase_one_character(line);
            return true;
        default:
            return true;
    }
}

unsigned char PseudoTerminal::receive_byte() const {
    unsigned char c;
    ssize_t count = read(master_handle, &c, 1);

    std::cout << std::hex << "Recv: " << static_cast<unsigned short>(c) << std::dec << std::endl;

    if (count < 0) {
        throw std::runtime_error("Error reading from PseudoTerminal");
    }
    if (c == 0) {
        throw EndOfFileError();
    }

    return c;
}

std::variant<ControlCode, unsigned char> PseudoTerminal::read_code() const {
    unsigned char code = receive_byte();

    if (code >= 0x20 && code <= 0x7F) {
        return code;
    }

    switch (code) {
        case 0x07:
            return ControlCode::BELL;
        case 0x08:
            return ControlCode::BACKSPACE;
        case 0x09:
            return ControlCode::TAB;
        case 0x0A:
            return ControlCode::LINE_FEED;
        case 0x0C:
            return ControlCode::FORM_FEED;
        case 0x0D:
            return ControlCode::CARRIAGE_RETURN;
        case 0x1B:
            return read_escape_sequence();
        default:
            return ControlCode::UNKNOWN;
    }
}

ControlCode PseudoTerminal::read_escape_sequence() const {
    unsigned char second_byte = receive_byte();

    if (second_byte < 0x40 || second_byte > 0x5F) {
        return ControlCode::ERROR;
    }

    // CSI (Control Sequence Introducer)
    if (second_byte == '[') {
        return read_csi_sequence();
    } else {
        return ControlCode::UNKNOWN;
    }
}

ControlCode PseudoTerminal::read_csi_sequence() const {
    unsigned char b = receive_byte();

    // CSI sequence looks like ESC [ PBs IBs FB
    // We won't be using parameter and intermediate bytes, so
    // we'll just skip them

    // parameter bytes
    while (b >= 0x30 && b <= 0x3F) {
        b = receive_byte();
    }

    // intermediate bytes
    while (b >= 0x20 && b <= 0x2F) {
        b = receive_byte();
    }

    unsigned char final_byte = b;
    if (final_byte < 0x40 || final_byte > 0x7E) {
        return ControlCode::ERROR;
    }

    switch (final_byte) {
        case 'A':
            return ControlCode::CURSOR_UP_1;
        case 'B':
            return ControlCode::CURSOR_DOWN_1;
        default:
            return ControlCode::UNKNOWN;
    }
}

std::string PseudoTerminal::change_to_previous(const std::string &current) {
    std::string prev = history.previous();

    if (prev.empty()) {
        return current;
    }

    send_erase_sequence(current.size());
    send(prev);
    return prev;
}

std::string PseudoTerminal::change_to_next(const std::string &current) {
    std::string next = history.next();

    if (next.empty()) {
        return current;
    }

    send_erase_sequence(current.size());

    send(next);
    return next;
}

void PseudoTerminal::erase_one_character(std::string &line) {
    if (line.empty()) {
        return;
    }

    line.pop_back();
    send_erase_sequence(1);
}

void PseudoTerminal::send_erase_sequence(std::size_t count) const {
    for (std::size_t i = 0; i < count; i++) {
        send(0x08); // backspace
        send(0x20); // space
        send(0x08); // backspace
    }
}
