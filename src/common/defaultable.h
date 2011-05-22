#ifndef DEFAULTPROVIDER_H
#define DEFAULTPROVIDER_H

#include "nullable.h"

namespace common
{
template<class T> class Defaultable
{
public:
    Defaultable(const T &def) : m_default(def) { }

    inline T defaultValue() const { return m_default; }
    inline bool isSet() const { return m_value.hasValue(); }
    inline void reset() { m_value.unset(); }
    inline void set(T v) { m_value.set(v); }

    const T & value() const
    {
        return (m_value.hasValue()) ? m_value.value() : m_default;
    }

    operator T() const { return value(); }

    const T & operator=(const T &rhs)
    {
        set(rhs);
        return rhs;
    }

private:
    const T m_default;
    Nullable<T> m_value;
};
} /* namespace asl */

#endif /* DEFAULTPROVIDER_H */

