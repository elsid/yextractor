#include <yamail/yextractor/detail/parser.hpp>

#include <gtest/gtest.h>

namespace {

using namespace testing;

using namespace yamail::yextractor::detail;

struct ParserTest : Test {};

TEST(ParserTest, parse_string_should_succeed) {
    const Parser<std::string> parse;
    std::string result;
    const auto errors = parse(result, "string");
    EXPECT_EQ(errors, Errors());
    EXPECT_EQ(result, "string");
}

} // namespace
