#ifndef YAMAIL_YEXTRACTOR_EVERY_HPP
#define YAMAIL_YEXTRACTOR_EVERY_HPP

#include <tuple>

namespace yamail {
namespace yextractor {

template <class ... Values>
struct Every {
    using Value = std::tuple<Values ...>;
    Value values;
};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_EVERY_HPP
