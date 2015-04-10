#ifndef YAMAIL_YEXTRACTOR_ANY_HPP
#define YAMAIL_YEXTRACTOR_ANY_HPP

namespace yamail {
namespace yextractor {

template <class ... Values>
struct Any {
    using Value = std::tuple<Values ...>;
    Value values;
};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_ANY_HPP
