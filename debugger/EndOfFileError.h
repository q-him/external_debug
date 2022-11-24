//
// Created by avkozyrev on 24.11.22.
//

#ifndef EXTERNAL_DEBUG_ENDOFFILEERROR_H
#define EXTERNAL_DEBUG_ENDOFFILEERROR_H

#include <exception>

class EndOfFileError : public std::exception {
public:
    const char *what() const noexcept override;
};


#endif //EXTERNAL_DEBUG_ENDOFFILEERROR_H
