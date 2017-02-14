#ifndef YMAIL_EXTRACTOR_WITHOUT_HPP
#define YMAIL_EXTRACTOR_WITHOUT_HPP

#include <yamail/yextractor/parameter.hpp>

namespace yamail {
namespace yextractor {

template <class T>
struct Without {
    using Type = T;
    using Value = detail::Value<Type>;

    Value value;
};

template <class Tag>
struct Without<Parameter<Tag>> : Parameter<Tag> {};

} // namespace yextractor
} // namespace yamail

#endif // YMAIL_EXTRACTOR_WITHOUT_HPP
