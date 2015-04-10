#ifndef YAMAIL_YEXTRACTOR_FIRST_HPP
#define YAMAIL_YEXTRACTOR_FIRST_HPP

namespace yamail {
namespace yextractor {

template <class ... Values>
struct First {
    using Value = std::tuple<Values ...>;
    Value values;
};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_FIRST_HPP
