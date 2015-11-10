#include <gtest/gtest.h>

#include <yamail/yextractor/any.hpp>
#include <yamail/yextractor/every.hpp>
#include <yamail/yextractor/optional.hpp>
#include <yamail/yextractor/parameter.hpp>
#include <yamail/yextractor/required.hpp>
#include <yamail/yextractor/first.hpp>
#include <yamail/yextractor/detail/to_string.hpp>

namespace {

using namespace testing;

using namespace yamail::yextractor;
using namespace yamail::yextractor::detail;

DEFINE_PARAMETER(A, std::string, "a")
DEFINE_PARAMETER(B, std::string, "b")

TEST(ToStringTest, to_string_expression_of_user_defined_parameter_should_return_quoted_name) {
    A x;
    EXPECT_EQ(toString(x), "'a'");
}

TEST(ToStringTest, to_string_expression_of_required_parameter_should_return_quoted_name) {
    Required<A> x;
    EXPECT_EQ(toString(x), "'a'");
}

TEST(ToStringTest, to_string_expression_of_optional_parameter_should_return_quoted_name_with_prefix) {
    Optional<A> x;
    EXPECT_EQ(toString(x), "optional 'a'");
}

TEST(ToStringTest, to_string_expression_of_every_should_return_enbraced_list_of_params_strings_separeted_by_and) {
    Every<A, B> x;
    EXPECT_EQ(toString(x), "('a' and 'b')");
}

TEST(ToStringTest, to_string_expression_of_any_should_return_enbraced_list_of_params_strings_separeted_by_or) {
    Any<A, B> x;
    EXPECT_EQ(toString(x), "('a' or 'b')");
}

TEST(ToStringTest, to_string_expression_of_first_should_return_enbraced_list_of_params_strings_separeted_by_or) {
    First<A, B> x;
    EXPECT_EQ(toString(x), "('a' or 'b')");
}

TEST(ToStringTest, to_string_expression_of_every_with_optional_should_return_enbraced_list_of_params_strings_separeted_by_and_optional_with_prefix) {
    Every<A, Optional<B>> x;
    EXPECT_EQ(toString(x), "('a' and optional 'b')");
}

} // namespace
