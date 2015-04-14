#ifndef YAMAIL_YEXTRACTOR_EXCLUDE_HPP
#define YAMAIL_YEXTRACTOR_EXCLUDE_HPP

#include <tuple>

namespace yamail {
namespace yextractor {
namespace detail {

template <const std::size_t left, class Parameters, class Container>
struct Exclude {
    using Next = Exclude<left - 1, Parameters, Container>;

    static void exclude(Container& container) {
        container.erase(std::tuple_element<left - 1, Parameters>::type::name());
        Next::exclude(container);
    }
};

template <class Parameters, class Container>
struct Exclude<1, Parameters, Container> {
    static void exclude(Container& container) {
        container.erase(std::tuple_element<0, Parameters>::type::name());
    }
};

template <class Parameters, class Container>
struct Exclude<0, Parameters, Container> {
    static void exclude(Container&) {}
};

} // namespace detail

template <class Parameters, class Container>
void exclude(Container& container) {
    static constexpr auto size = std::tuple_size<Parameters>::value;
    detail::Exclude<size, Parameters, Container>::exclude(container);
}

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_EXCLUDE_HPP
