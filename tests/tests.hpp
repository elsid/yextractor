#ifndef INTERNAL_TESTS_HPP
#define INTERNAL_TESTS_HPP

#include <ostream>

#include <yamail/yextractor.hpp>

namespace tests {

using namespace yamail::yextractor;
using namespace yamail::yextractor::detail;

template <class ... Values>
std::ostream& operator <<(std::ostream &stream,
                          const std::tuple<Values ...>& values) {
    return stream << toString(values);
}

inline std::ostream& operator <<(std::ostream &stream, const Errors& errors) {
    stream << "[" << std::endl;
    for (const auto& error : errors) {
        stream << "    " << error << std::endl;
    }
    return stream << "]" << std::endl;;
}

template <class Key, class Value>
std::ostream& operator <<(std::ostream &stream,
                          const std::map<Key, Value>& values) {
    for (const auto& pair : values) {
        stream << pair.first << ": " << pair.second << std::endl;
    }
    return stream;
}

template <class T>
inline std::ostream& operator <<(std::ostream &stream, const Parameter<T>& value) {
    return value.initialized() ? stream << value.get() : stream << "none";
}

template <class T>
inline std::ostream& operator <<(std::ostream &stream, const Value<T>& value) {
    return value.initialized() ? stream << value.get() : stream << "none";
}

} // namespace tests

#endif // INTERNAL_TESTS_HPP
