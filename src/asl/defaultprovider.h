#ifndef DEFAULTPROVIDER_H
#define DEFAULTPROVIDER_H

namespace asl
{
template<class T> class DefaultProvider
{
public:
    DefaultProvider(T def) : m_default(def), m_value(def), m_isSet(false) { }

    inline T defaultValue() const { return m_default; }
    inline bool isSet() const { return m_isSet; }
    inline void reset() { m_value = m_default; m_isSet = false; }
    inline void set(T v) { m_value = v; m_isSet = true; }
    inline T value() const { return m_value; }

private:
    const T m_default;
    T m_value;
    bool m_isSet;
};
} /* namespace asl */

#endif /* DEFAULTPROVIDER_H */

