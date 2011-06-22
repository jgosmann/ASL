#ifndef NOVALUEEXCEPTION_H
#define NOVALUEEXCEPTION_H

#include <stdexcept>
#include <string>

namespace common
{
class NoValueException : public std::logic_error
{
public:
    NoValueException(const char *msg = "Tried to access non existent value.")
            : std::logic_error(msg)
    {
    }

    NoValueException(const std::string &msg) : std::logic_error(msg) { }
};
} /* namespace common */

#endif /* NOVALUEEXCEPTION_H */

