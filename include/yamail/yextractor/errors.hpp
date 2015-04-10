#ifndef YAMAIL_YEXTRACTOR_ERRORS_HPP
#define YAMAIL_YEXTRACTOR_ERRORS_HPP

#include <deque>

namespace yamail {
namespace yextractor {

class Errors {
public:
    using Messages = std::deque<std::string>;
    using const_iterator = Messages::const_iterator;

    Errors() = default;
    Errors(const std::string& error) : errors_({error}) {}
    Errors(const Messages& errors) : errors_(errors) {}
    template <class ... Values>
    Errors(const std::initializer_list<std::string>& errors) : errors_(errors) {}

    Errors& operator &&(const Errors& other) {
        if (!empty() || !other.empty()) {
            addErrors(other.errors_);
        } else {
            errors_.clear();
        }
        return *this;
    }

    Errors& operator ||(const Errors& other) {
        if (!empty() && !other.empty()) {
            addErrors(other.errors_);
        } else {
            errors_.clear();
        }
        return *this;
    }

    bool operator ==(const Errors& other) const {
        return errors_ == other.errors_;
    }

    bool empty() const {
        return errors_.empty();
    }

    const std::deque<std::string>& messages() const {
        return errors_;
    }

    const_iterator begin() const { return errors_.begin(); }
    const_iterator end() const { return errors_.end(); }

private:
    std::deque<std::string> errors_;

    void addErrors(const std::deque<std::string>& other) {
        std::copy(other.begin(), other.end(), std::front_inserter(errors_));
    }
};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_ERRORS_HPP
