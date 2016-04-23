#ifndef YAMAIL_YEXTRACTOR_DETAIL_FLAT_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_FLAT_HPP

#include <yamail/yextractor/any.hpp>
#include <yamail/yextractor/every.hpp>
#include <yamail/yextractor/first.hpp>
#include <yamail/yextractor/optional.hpp>
#include <yamail/yextractor/parameter.hpp>
#include <yamail/yextractor/required.hpp>

namespace yamail {
namespace yextractor {
namespace detail {

template <const std::size_t left, class ... Values>
struct Flat;

template <class Value>
std::tuple<Value> flatValue(Value&& value) {
    return std::make_tuple(std::move(value));
}

template <class Tag>
std::tuple<Parameter<Tag>> flatValue(Parameter<Tag>&& value) {
    return std::make_tuple(std::move(value));
}

template <class Tag>
std::tuple<Parameter<Tag>> flatValue(Required<Parameter<Tag>>&& value) {
    return std::make_tuple(Parameter<Tag>(std::move(value)));
}

template <class Tag>
std::tuple<Parameter<Tag>> flatValue(Optional<Parameter<Tag>>&& value) {
    return std::make_tuple(Parameter<Tag>(std::move(value)));
}

template <class ... Values>
auto flat(std::tuple<Values ...>&& values)
        -> decltype(Flat<sizeof ... (Values), Values ...>::flat(std::move(values)));

template <class ... Values>
auto flat(Any<Values ...>&& values) -> decltype(flat(std::move(values.values)));

template <class ... Values>
auto flat(First<Values ...>&& values) -> decltype(flat(std::move(values.values)));

template <class ... Values>
auto flat(Every<Values ...>&& values) -> decltype(flat(std::move(values.values)));

template <class ... Values>
auto flatValue(Any<Values ...>&& values) -> decltype(flat(std::move(values.values))) {
    return flat(std::move(values.values));
}

template <class ... Values>
auto flatValue(First<Values ...>&& values) -> decltype(flat(std::move(values.values))) {
    return flat(std::move(values.values));
}

template <class ... Values>
auto flatValue(Every<Values ...>&& values) -> decltype(flat(std::move(values.values))) {
    return flat(std::move(values.values));
}

template <const std::size_t left, class ... Values>
struct Flat {
    using Next = Flat<left - 1, Values ...>;
    using Tuple = typename std::tuple<Values ...>;
    using Element = typename std::tuple_element<left - 1, Tuple>::type;

    static auto flat(Tuple&& values)
            -> decltype(std::tuple_cat(Next::flat(std::move(values)),
                                       flatValue(std::move(std::get<left - 1>(values))))) {
        return std::tuple_cat(Next::flat(std::move(values)),
                              flatValue(std::move(std::get<left - 1>(values))));
    }
};

template <class ... Values>
struct Flat<1, Values ...> {
    using Tuple = typename std::tuple<Values ...>;
    using Element = typename std::tuple_element<0, Tuple>::type;

    static auto flat(Tuple&& values)
            -> decltype(flatValue(std::move(std::get<0>(values)))) {
        return flatValue(std::move(std::get<0>(values)));
    }
};

template <class ... Values>
struct Flat<0, Values ...> {
    static std::tuple<> flat(std::tuple<Values ...>&&) {
        return std::tuple<>();
    }
};

template <class Value>
auto flat(Required<Value>&& value) -> decltype(flatValue(std::move(value))) {
    return flatValue(std::move(value));
}

template <class Value>
auto flat(Optional<Value>&& value) -> decltype(flatValue(std::move(value))) {
    return flatValue(std::move(value));
}

template <class ... Values>
auto flat(std::tuple<Values ...>&& values)
        -> decltype(Flat<sizeof ... (Values), Values ...>::flat(std::move(values))) {
    return Flat<sizeof ... (Values), Values ...>::flat(std::move(values));
}

template <class ... Values>
auto flat(Any<Values ...>&& values) -> decltype(flat(std::move(values.values))) {
    return flat(std::move(values.values));
}

template <class ... Values>
auto flat(First<Values ...>&& values) -> decltype(flat(std::move(values.values))) {
    return flat(std::move(values.values));
}

template <class ... Values>
auto flat(Every<Values ...>&& values) -> decltype(flat(std::move(values.values))) {
    return flat(std::move(values.values));
}

} // namespace detail
} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_DETAIL_FLAT_HPP
