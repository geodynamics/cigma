#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <stdexcept>

class BadConversion : public std::runtime_error
{
public:
    BadConversion(const std::string& s) : std::runtime_error(s) {}
};

template <typename T>
inline std::string stringify(const T& x)
{
    std::ostringstream o;
    if (!(o << x))
    {
        throw BadConversion(std::string("stringify(") + typeid(x).name() + ")");
    }
    return o.str();
}

template <typename T>
inline void convert(const std::string& s, T& x,
                    bool failIfLeftOverChars = true)
{
    std::istringstream i(s);
    char c;
    if (!(i >> x) || (failIfLeftOverChars && i.get(c)))
    {
        throw BadConversion(s);
    }
}

template <typename T>
inline T convertTo(const std::string& s,
                   bool failIfLeftOverChars = true)
{
    T x;
    convert(s, x, failIfLeftOverChars);
    return x;
}

#endif
