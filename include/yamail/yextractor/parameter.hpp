#ifndef YAMAIL_YEXTRACTOR_PARAMETER_HPP
#define YAMAIL_YEXTRACTOR_PARAMETER_HPP

#include <boost/optional.hpp>

#include <yamail/yextractor/detail/get.hpp>
#include <yamail/yextractor/detail/parser.hpp>

namespace yamail {
namespace yextractor {

template <class Tag>
struct Parameter {
    using Type = typename Tag::Value;
    using Parser = typename Tag::Parser;
    using Value = boost::optional<Type>;

    Value value;
    Parser parser;

    Parameter() = default;
    Parameter(const Type& value) : value(value) {}
    Parameter(Type&& value) : value(std::move(value)) {}
    Parameter(const Value& value) : value(value) {}
    Parameter(Value&& value) : value(std::move(value)) {}
    template <class ... Values>
    Parameter(const std::tuple<Values ...>& values)
            : value(detail::get<Parameter>(values)) {}
    Parameter(const Parameter &) = delete;
    Parameter(Parameter &&) = default;

    static std::string name() { return Tag::name; }
    const Type& get() const { return value.get(); }
    operator Type() const { return get(); }

    bool operator == (const Parameter& other) const {
        return value == other.value;
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
