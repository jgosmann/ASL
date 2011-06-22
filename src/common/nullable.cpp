
#include "nullable.h"

using namespace common;

template<class T> Nullable<T>::Nullable() : m_hasValue(false)
{
}

template<class T> Nullable<T>::Nullable(const T &value)
        : m_hasValue(true), m_value(new T(value))
{
}

template<class T> Nullable<T>::Nullable(const Nullable<T> &other)
        : m_hasValue(false)
{
    if (other.hasValue()) {
        set(other.value());
    }
}

template<class T> Nullable<T>::~Nullable()
{
    if (m_hasValue) {
        delete m_value;
    }
}

template<class T> void Nullable<T>::set(const T &value)
{
    if (m_hasValue) {
        delete m_value;
    }

    m_value = new T(value);
    m_hasValue = true;
}

template<class T> void Nullable<T>::unset()
{
    if (m_hasValue) {
        delete m_value;
    }
    m_hasValue = false;
}

template<class T> const T & Nullable<T>::value() const throw(NoValueException)
{
    if (!m_hasValue) {
        throw NoValueException();
    }
    return *m_value;
}

template<class T> const T & Nullable<T>::valueOrDefault(const T &def) const
{
    return hasValue() ? value() : def;
}


template<class T> Nullable<T> & Nullable<T>::operator=(const Nullable<T> &rhs)
{
    if (this == &rhs) {
        return *this;
    }

    if (rhs.hasValue()) {
        set(rhs.value());
    } else {
        unset();
    }

    return *this;
}

