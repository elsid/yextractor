#ifndef YAMAIL_YEXTRACTOR_EXTRACTOR_HPP
#define YAMAIL_YEXTRACTOR_EXTRACTOR_HPP

#include <yamail/yextractor/detail/fill.hpp>
#include <yamail/yextractor/detail/flat.hpp>

namespace yamail {
namespace yextractor {

class Extractor {
public:
    template <class T, class Source>
    auto get(const Source& source) -> decltype(detail::flat(T())) {
        T value;
        errors(std::move(detail::Fill<Source>(source).fill(value)));
        return detail::flat(std::move(value));
    }

    const Errors& errors() const {
        return errors_;
    }

private:
    Errors errors_;

    void errors(Errors&& value) {
        errors_ = std::move(value);
    }
};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_EXTRACTOR_HPP
