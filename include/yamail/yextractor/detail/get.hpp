#ifndef YAMAIL_YEXTRACTOR_DETAIL_GET_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_GET_HPP

#include <tuple>

namespace yamail {
namespace yextractor {
namespace detail {

template <class T, const std::size_t position, class ... Values>
struct GetPosition {
    static constexpr auto value = position;
};

template <class T, const std::size_t position, class ... Values>
struct GetPosition<T, position, T, Values ...> {
    static constexpr auto value = position;
};

template <class T, const std::size_t position, class U, class ... Values>
struct GetPosition<T, position, U, Values ...> {
    static constexpr std::size_t value = GetPosition<T, position + 1, Values ...>::value;
};

template <class Parameter, class ... Values>
const typename Parameter::Value& get(const std::tuple<Values ...>& t) {
    return std::get<GetPosition<Parameter, 0, Values ...>::value>(t).value;
}

template <class Parameter, class ... Values>
typename Parameter::Value&& get(std::tuple<Values ...>& t) {
    return std::move(std::get<GetPosition<Parameter, 0, Values ...>::value>(t).value);
}

} // namespace detail
} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_DETAIL_GET_HPP
