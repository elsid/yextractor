#ifndef YAMAIL_YEXTRACTOR_DETAIL_FILL_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_FILL_HPP

#include <yamail/yextractor/any.hpp>
#include <yamail/yextractor/every.hpp>
#include <yamail/yextractor/optional.hpp>
#include <yamail/yextractor/parameter.hpp>
#include <yamail/yextractor/required.hpp>
#include <yamail/yextractor/errors.hpp>
#include <yamail/yextractor/first.hpp>
#include <yamail/yextractor/detail/to_string.hpp>

namespace yamail {
namespace yextractor {
namespace detail {

template <const std::size_t left, class Source, class ... Values>
struct FillAny;

template <const std::size_t left, class Source, class ... Values>
struct FillFirst;

template <const std::size_t left, class Source, class ... Values>
struct FillEvery;

template <class Source>
struct Fill {
    const Source& source;

    Fill(const Source& source) : source(source) {}

    template <class ... Values>
    Errors fill(Any<Values ...>& value) const {
        auto errors = fillAny<sizeof ...(Values)>(value.values);
        if (errors.empty()) {
            return errors;
        }
        return errors || Errors("not any of parameters " + toString(value)
                                + " are present");
    }

    template <class ... Values>
    Errors fill(First<Values ...>& value) const {
        auto errors = fillFirst<sizeof ...(Values)>(value.values);
        if (errors.empty()) {
            return errors;
        }
        return errors || Errors("not any of parameters " + toString(value)
                                + " are present when looking for first");
    }

    template <class ... Values>
    Errors fill(Every<Values ...>& value) const {
        auto errors = fillEvery<sizeof ...(Values)>(value.values);
        if (errors.empty()) {
            return errors;
        }
        return errors || Errors("not every of parameters " + toString(value)
                                + " are present");
    }

    template <class T>
    Errors fill(Required<T>& required) const {
        return fill(required.value, required.name(), required.parser,
                    Errors("parameter '" + required.name() + "' not found"));
    }

    template <class T>
    Errors fill(Optional<T>& optional) const {
        return fill(optional.value, optional.name(), optional.parser, Errors());
    }

    template <class T>
    Errors fill(Parameter<T>& parameter) const {
        return fill(parameter.value, parameter.name(), parameter.parser,
                    Errors("parameter '" + parameter.name() + "' not found"));
    }

    template <class T, class Parser>
    Errors fill(detail::Value<T>& value, const std::string& name, Parser parse, Errors ifNotFound) const {
        const auto it = source.find(name);
        if (it == source.end()) {
            return ifNotFound;
        } else {
            T parsedValue;
            const auto errors = parse(parsedValue, it->second);
            if (errors.empty()) {
                value = std::move(parsedValue);
            }
            return errors;
        }
    }

    template <const std::size_t left, class ... Values>
    Errors fillAny(std::tuple<Values ...>& values) const {
        return FillAny<left, Source, Values ...>(source).fill(values);
    }

    template <const std::size_t left, class ... Values>
    Errors fillFirst(std::tuple<Values ...>& values) const {
        return FillFirst<left, Source, Values ...>(source).fill(values);
    }

    template <const std::size_t left, class ... Values>
    Errors fillEvery(std::tuple<Values ...>& values) const {
        return FillEvery<left, Source, Values ...>(source).fill(values);
    }
};

template <const std::size_t left, class Source, class ... Values>
struct FillAny : Fill<Source> {
    using Base = Fill<Source>;

    FillAny(const Source& source) : Base(source) {}

    Errors fill(std::tuple<Values ...>& values) const {
        return Base::fill(std::get<left - 1>(values))
                || FillAny<left - 1, Source, Values ...>(this->source).fill(values);
    }
};

template <class Source, class ... Values>
struct FillAny<1, Source, Values ...> : Fill<Source> {
    using Base = Fill<Source>;

    FillAny(const Source& source) : Base(source) {}

    Errors fill(std::tuple<Values ...>& values) const {
        return Base::fill(std::get<0>(values));
    }
};

template <class Source, class ... Values>
struct FillAny<0, Source, Values ...> : Fill<Source> {
    using Base = Fill<Source>;

    FillAny(const Source& source) : Base(source) {}

    Errors fill(std::tuple<Values ...>&) const {
        return Errors("no values");
    }
};

template <const std::size_t left, class Source, class ... Values>
struct FillFirst : Fill<Source> {
    using Base = Fill<Source>;

    FillFirst(const Source& source) : Base(source) {}

    Errors fill(std::tuple<Values ...>& values) const {
        auto errors = FillFirst<left - 1, Source, Values ...>(this->source).fill(values);
        if (errors.empty()) {
            return errors;
        }
        return Base::fill(std::get<left - 1>(values)) || std::move(errors);
    }
};

template <class Source, class ... Values>
struct FillFirst<1, Source, Values ...> : Fill<Source> {
    using Base = Fill<Source>;

    FillFirst(const Source& source) : Base(source) {}

    Errors fill(std::tuple<Values ...>& values) const {
        return Base::fill(std::get<0>(values));
    }
};

template <class Source, class ... Values>
struct FillFirst<0, Source, Values ...> : Fill<Source> {
    FillFirst(const Source& source) : Fill<Source>(source) {}

    Errors fill(std::tuple<Values ...>&) const {
        return Errors("no values");
    }
};

template <const std::size_t left, class Source, class ... Values>
struct FillEvery : Fill<Source> {
    using Base = Fill<Source>;

    FillEvery(const Source& source) : Base(source) {}

    Errors fill(std::tuple<Values ...>& values) const {
        return Base::fill(std::get<left - 1>(values))
            && FillEvery<left - 1, Source, Values ...>(this->source).fill(values);
    }
};

template <class Source, class ... Values>
struct FillEvery<1, Source, Values ...> : Fill<Source> {
    using Base = Fill<Source>;

    FillEvery(const Source& source) : Base(source) {}

    Errors fill(std::tuple<Values ...>& values) const {
        return Base::fill(std::get<0>(values));
    }
};

template <class Source, class ... Values>
struct FillEvery<0, Source, Values ...> : Fill<Source> {
    FillEvery(const Source& source) : Fill<Source>(source) {}

    Errors fill(std::tuple<Values ...>&) const {
        return Errors("no values");
    }
};

} // namespace detail
} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_DETAIL_FILL_HPP
