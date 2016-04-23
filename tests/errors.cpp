#include <yamail/yextractor/errors.hpp>

#include <gtest/gtest.h>

#include <algorithm>

namespace {

using namespace testing;

using namespace yamail::yextractor;

struct ErrorsTest : Test {};

TEST(ErrorsTest, create_default_should_be_empty) {
    const Errors errors;
    EXPECT_EQ(errors.empty(), true);
}

TEST(ErrorsTest, create_from_string_should_contains_one) {
    const Errors errors("error");
    EXPECT_EQ(std::distance(errors.begin(), errors.end()), 1);
}

TEST(ErrorsTest, create_from_collection_then_messages_should_return_equal) {
    const Errors::Messages messages({"error"});
    const Errors errors(messages);
    const auto& errorsMessages = errors.messages();
    EXPECT_EQ(errorsMessages, messages);
}

TEST(ErrorsTest, two_empty_should_be_equal) {
    EXPECT_EQ(Errors(), Errors());
}

} // namespace
