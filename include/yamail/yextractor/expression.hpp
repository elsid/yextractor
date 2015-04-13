#ifndef YAMAIL_YEXTRACTOR_EXPRESSION_HPP
#define YAMAIL_YEXTRACTOR_EXPRESSION_HPP

#include <map>

#include <yamail/yextractor/extractor.hpp>

namespace yamail {
namespace yextractor {

template <class T>
struct Expression {
    using Type = decltype(Extractor().get<T>(std::map<std::string, std::string>()));
};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_EXPRESSION_HPP
