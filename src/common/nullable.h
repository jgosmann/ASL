#ifndef NULLABLE_H
#define NULLABLE_H

#include "novalueexception.h"

namespace common
{
template<class T> class Nullable
{
public:
    Nullable() : m_hasValue(false) { }
    Nullable(const T &value) : m_value(value), m_hasValue(true) { }

    inline bool hasValue() const { return m_hasValue; }
    inline void set(const T &value) { m_value = value; m_hasValue = true; }
    inline void unset() { m_hasValue = false; }

    const T & value() const throw(NoValueException)
    {
        if (!m_hasValue) {
            throw NoValueException();
        }
        return m_value;
    }

    const T & valueOrDefault(const T &def) const
    {
        return hasValue() ? value() : def;
    }

    operator T() const throw(NoValueException) { return value(); }

private:
    T m_value;
    bool m_hasValue;
};
} /* namespace asl */

#endif /* NULLABLE_H */

