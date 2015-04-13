#ifndef YAMAIL_YEXTRACTOR_DETAIL_TO_STRING_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_TO_STRING_HPP

#include <string>

namespace yamail {
namespace yextractor {

template <class Tag>
std::string toString(const Parameter<Tag>& value) {
    return "'" + value.name() + "'";
}

namespace detail {

struct Separator {
    struct Comma {
        static constexpr const char *value = ", ";
    };
    struct And {
        static constexpr const char *value = " and ";
    };
    struct Or {
        static constexpr const char *value = " or ";
    };
};

template <const std::size_t left, class Separator, class ... Values>
struct ToString {
    using Next = ToString<left - 1, Separator, Values ...>;

    static std::string convert(const std::tuple<Values ...>& values) {
        return Next::convert(values) + Separator::value
                + toString(std::get<left - 1, Values ...>(values));
    }
};

template <class Separator, class ... Values>
struct ToString<1, Separator, Values ...> {
    static std::string convert(const std::tuple<Values ...>& values) {
        return toString(std::get<0>(values));
    }
};

template <class Separator, class ... Values>
struct ToString<0, Separator, Values ...> {
    static std::string convert(const std::tuple<Values ...>&) {
        return std::string();
    }
};

} // namespace detail

template <class ... Values>
std::string toString(const std::tuple<Values ...>& values) {
    using namespace detail;
    return "(" + ToString<sizeof ... (Values), Separator::Comma, Values ...>::convert(values) + ")";
}

template <class ... Values>
std::string toString(const Any<Values ...>& value) {
    using namespace detail;
    return "(" + ToString<sizeof ... (Values), Separator::Or, Values ...>::convert(value.values) + ")";
}

template <class ... Values>
std::string toString(const First<Values ...>& value) {
    using namespace detail;
    return "(" + ToString<sizeof ... (Values), Separator::Or, Values ...>::convert(value.values) + ")";
}

template <class ... Values>
std::string toString(const Every<Values ...>& value) {
    using namespace detail;
    return "(" + ToString<sizeof ... (Values), Separator::And, Values ...>::convert(value.values) + ")";
}

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_DETAIL_TO_STRING_HPP
