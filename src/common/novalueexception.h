#ifndef NOVALUEEXCEPTION_H
#define NOVALUEEXCEPTION_H

#include <stdexcept>

namespace common
{
class NoValueException : public std::logic_error
{
public:
    NoValueException() : std::logic_error("Tried to access non existent value.")
    {
    }
};
} /* namespace common */

#endif /* NOVALUEEXCEPTION_H */

