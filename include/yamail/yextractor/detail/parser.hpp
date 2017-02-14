#ifndef YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP

#include <yamail/yextractor/errors.hpp>

#include <sstream>
#include <type_traits>

namespace yamail {
namespace yextractor {
namespace detail {

template <class T, class Enabled = void>
struct Parser {};

template <>
struct Parser<std::string, void> {
    Errors operator()(std::string& dst, const std::string& src) const {
        dst = src;
        return Errors();
    }
};

template <>
struct Parser<bool, void> {
    Errors operator()(bool& dst, const std::string& src) const {
        std::istringstream stream(src);
        stream >> std::boolalpha >> dst;
        return stream.fail() ? Errors("fail to parse bool value") : Errors();
    }
};

template <class T>
struct Parser<T, typename std::enable_if<std::is_arithmetic<T>::value, void>::type> {
    Errors operator()(T& dst, const std::string& src) const {
        std::istringstream stream(src);
        stream >> dst;
        return stream.fail() ? Errors("fail to parse arithmetic value") : Errors();
    }
};

} // namespace detail
} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP
