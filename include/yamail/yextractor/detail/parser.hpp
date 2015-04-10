#ifndef YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP

#include <yamail/yextractor/errors.hpp>

#include <boost/optional.hpp>

namespace yamail {
namespace yextractor {
namespace detail {

template <class T>
struct Parser {
    Errors operator()(T& dst, const std::string& src) const {
        dst = T(src);
        return Errors();
    }
};

template <class T>
struct Parser<boost::optional<T>> {
    Errors operator()(boost::optional<T>& dst, const std::string& src) const {
        dst = Parser<T>()(src);
        return Errors();
    }
};

template <>
struct Parser<bool> {
    Errors operator()(bool& dst, const std::string& src) const {
        dst = src == "y";
        return Errors();
    }
};

template <>
struct Parser<std::string> {
    Errors operator()(std::string& dst, const std::string& src) const {
        dst = src;
        return Errors();
    }
};

} // namespace detail
} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP
