#include <yamail/yextractor/detail/parser.hpp>

#include <gtest/gtest.h>

namespace {

using namespace testing;

using namespace yamail::yextractor;
using namespace yamail::yextractor::detail;

struct ParserTest : Test {};

TEST(ParserTest, parse_string_should_succeed) {
    const Parser<std::string> parse;
    std::string result;
    const auto errors = parse(result, "string");
    EXPECT_EQ(errors, Errors());
    EXPECT_EQ(result, "string");
}

TEST(ParserTest, parse_bool_should_succeed) {
    const Parser<bool> parse;
    bool result;
    const auto errors = parse(result, "true");
    EXPECT_EQ(errors, Errors());
    EXPECT_EQ(result, true);
}

TEST(ParserTest, parse_char_should_succeed) {
    const Parser<char> parse;
    char result;
    const auto errors = parse(result, "a");
    EXPECT_EQ(errors, Errors());
    EXPECT_EQ(result, 'a');
}

TEST(ParserTest, parse_int_should_succeed) {
    const Parser<int> parse;
    int result;
    const auto errors = parse(result, "42");
    EXPECT_EQ(errors, Errors());
    EXPECT_EQ(result, 42);
}

TEST(ParserTest, parse_double_should_succeed) {
    const Parser<double> parse;
    double result;
    const auto errors = parse(result, "42e13");
    EXPECT_EQ(errors, Errors());
    EXPECT_EQ(result, 42e13);
}

} // namespace
