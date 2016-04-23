#ifndef YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_PARSER_HPP

#include <yamail/yextractor/errors.hpp>

namespace yamail {
namespace yextractor {
namespace detail {

template <class T>
struct Parser;

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
