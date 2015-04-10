#ifndef YMAIL_EXTRACTOR_OPTIONAL_HPP
#define YMAIL_EXTRACTOR_OPTIONAL_HPP

#include <boost/optional.hpp>

#include <yamail/yextractor/parameter.hpp>

namespace yamail {
namespace yextractor {

template <class T>
struct Optional {
    using Type = T;
    using Value = boost::optional<Type>;

    Value value;
};

template <class Tag>
struct Optional<Parameter<Tag>> : Parameter<Tag> {};

} // namespace yextractor
} // namespace yamail

#endif // YMAIL_EXTRACTOR_OPTIONAL_HPP
