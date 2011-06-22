#ifndef NULLABLE_H
#define NULLABLE_H

#include "novalueexception.h"

namespace common
{
template<class T> class Nullable
{
public:
    Nullable();
    Nullable(const T &value);
    Nullable(const Nullable<T> &other);
    ~Nullable();

    inline bool hasValue() const { return m_hasValue; }
    void set(const T &value);
    void unset();

    const T & value() const throw(NoValueException);
    const T & valueOrDefault(const T &def) const;

    Nullable<T> & operator=(const Nullable<T> &rhs);
    inline operator T() const throw(NoValueException) { return value(); }

private:
    bool m_hasValue;
    T *m_value;
};
} /* namespace asl */

#include "nullable.cpp"

#endif /* NULLABLE_H */

