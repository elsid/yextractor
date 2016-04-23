#ifndef YAMAIL_YEXTRACTOR_PARAMETER_HPP
#define YAMAIL_YEXTRACTOR_PARAMETER_HPP

#include <yamail/yextractor/detail/get.hpp>
#include <yamail/yextractor/detail/parser.hpp>
#include <yamail/yextractor/detail/value.hpp>

namespace yamail {
namespace yextractor {

template <class Tag>
struct Parameter {
    using Type = typename Tag::Value;
    using Parser = typename Tag::Parser;
    using Value = detail::Value<Type>;

    Value value;
    Parser parser;

    Parameter() = default;
    Parameter(Type&& value) : value(std::move(value)) {}
    template <class ... Values>
    Parameter(const std::tuple<Values ...>& values)
            : value(detail::get<Parameter>(values)) {}
    template <class ... Values>
    Parameter(std::tuple<Values ...>& values)
            : value(detail::get<Parameter>(values)) {}
    Parameter(const Parameter &) = delete;
    Parameter(Parameter &&) = default;

    virtual ~Parameter() {}

    static std::string name() { return Tag::name; }
    const Type& get() const { return value.get(); }
    Type&& take() { return value.take(); }
    bool initialized() const { return value.initialized(); }

    bool operator == (const Parameter& other) const {
        return value == other.value;
    }

    bool operator != (const Parameter& other) const {
        return value != other.value;
    }
};

} // namespace yextractor
} // namespace yamail

#define DEFINE_PARAMETER(ClassName, ValueType, parameterName) \
    struct ClassName##Tag { \
        using Value = ValueType; \
        using Parser = yamail::yextractor::detail::Parser<Value>; \
        static constexpr const char *name = parameterName; \
    }; \
    using ClassName = yamail::yextractor::Parameter<ClassName##Tag>;

#define DEFINE_PARAMETER_WITH_PARSER(ClassName, ValueType, parameterName, ParserType) \
    struct ClassName##Tag { \
        using Value = ValueType; \
        using Parser = ParserType; \
        static constexpr const char *name = parameterName; \
    }; \
    using ClassName = yamail::yextractor::Parameter<ClassName##Tag>;

#endif // YAMAIL_YEXTRACTOR_PARAMETER_HPP
