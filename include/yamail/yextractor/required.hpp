#ifndef YAMAIL_YEXTRACTOR_REQUIRED_HPP
#define YAMAIL_YEXTRACTOR_REQUIRED_HPP

#include <boost/optional.hpp>

namespace yamail {
namespace yextractor {

template <class T>
struct Required {
    using Type = T;
    using Value = boost::optional<Type>;

    Value value;
};

template <class Tag>
struct Required<Parameter<Tag>> : Parameter<Tag> {};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_REQUIRED_HPP
