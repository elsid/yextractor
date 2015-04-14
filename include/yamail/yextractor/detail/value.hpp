#ifndef YAMAIL_YEXTRACTOR_DETAIL_VALUE_HPP
#define YAMAIL_YEXTRACTOR_DETAIL_VALUE_HPP

#include <utility>

namespace yamail {
namespace yextractor {
namespace detail {

template <class T>
class Value {
public:
    using Type = T;

    Value() = default;
    Value(Type &&value) : initialized_(true), value(std::move(value)) {}
    Value(const Value&) = default;
    Value(Value&& other) : initialized_(other.initialized_), value(std::move(other.value)) {
        other.initialized_ = false;
    }

    Value& operator =(Type &&other) {
        value = std::move(other);
        initialized_ = true;
        return *this;
    }

    Value& operator =(Value &&other) {
        value = std::move(other.value);
        initialized_ = true;
        other.initialized_ = false;
        return *this;
    }

    bool operator ==(const Value &other) const {
        return initialized_ && other.initialized_
                ? value == other.value
                : initialized_ == other.initialized_;
    }

    bool operator !=(const Value &other) const {
        return !operator ==(other);
    }

    const Type &get() const { return value; }
    Type &get() { return value; }

    Type &&take() {
        initialized_ = false;
        return std::move(value);
    }

    bool initialized() const { return initialized_; }

private:
    bool initialized_ = false;
    Type value;
};

} // namespace detail
} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_DETAIL_VALUE_HPP
