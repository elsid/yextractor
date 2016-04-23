#ifndef YAMAIL_YEXTRACTOR_ERRORS_HPP
#define YAMAIL_YEXTRACTOR_ERRORS_HPP

#include <deque>
#include <string>

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

    Errors& operator &&(Errors&& other) {
        if (!empty() || !other.empty()) {
            addErrors(std::move(other.errors_));
        } else {
            errors_.clear();
        }
        return *this;
    }

    Errors& operator ||(Errors&& other) {
        if (!empty() && !other.empty()) {
            addErrors(std::move(other.errors_));
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

    const Messages& messages() const { return errors_; }

    const_iterator begin() const { return errors_.begin(); }
    const_iterator end() const { return errors_.end(); }

private:
    std::deque<std::string> errors_;

    void addErrors(std::deque<std::string>&& other) {
        for (auto&& error : other) {
            errors_.emplace_front(std::move(error));
        }
    }
};

} // namespace yextractor
} // namespace yamail

#endif // YAMAIL_YEXTRACTOR_ERRORS_HPP
