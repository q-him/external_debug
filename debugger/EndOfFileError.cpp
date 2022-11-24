//
// Created by avkozyrev on 24.11.22.
//

#include "EndOfFileError.h"

const char *EndOfFileError::what() const noexcept {
    return "EOF byte occurred";
}
