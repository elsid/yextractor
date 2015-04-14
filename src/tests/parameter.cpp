#include <map>

#include <gtest/gtest.h>

#include <yamail/yextractor.hpp>
#include <yamail/yextractor/expression.hpp>

#include <internal/tests.hpp>

namespace {

using namespace testing;
using namespace tests;

using namespace yamail::yextractor;
using namespace yamail::yextractor::detail;

using Source = std::map<std::string, std::string>;

DEFINE_PARAMETER(A, std::string, "a")
DEFINE_PARAMETER(B, std::string, "b")

struct ParameterTest : Test {};

TEST(ParameterTest, extract_parameter_and_copy_from_tuple_twice_should_be_two_equal_initialized_copies) {
    Extractor extractor;
    const Source source = {{"a", "value of a"}};
    const auto values = extractor.get<Required<A>>(source);
    ASSERT_TRUE(extractor.errors().empty());
    const A first(values);
    EXPECT_TRUE(first.initialized());
    EXPECT_EQ(first.get(), "value of a");
    const A second(values);
    EXPECT_TRUE(second.initialized());
    EXPECT_EQ(second.get(), "value of a");
    EXPECT_EQ(first, second);
}

TEST(ParameterTest, extract_parameter_and_move_from_tuple_twice_should_be_first_initialized_parameter_and_uninitialized_second) {
    Extractor extractor;
    const Source source = {{"a", "value of a"}};
    auto values = extractor.get<Required<A>>(source);
    ASSERT_TRUE(extractor.errors().empty());
    const A first(values);
    EXPECT_TRUE(first.initialized());
    EXPECT_EQ(first.get(), "value of a");
    const A second(values);
    EXPECT_FALSE(second.initialized());
    EXPECT_NE(first, second);
}

TEST(ParameterTest, extract_two_parameters_and_move_from_tuple_should_be_two_initialized_parameters) {
    Extractor extractor;
    const Source source = {{"a", "value of a"}, {"b", "value of b"}};
    auto values = extractor.get<Every<A, B>>(source);
    ASSERT_TRUE(extractor.errors().empty());
    const A a(values);
    EXPECT_TRUE(a.initialized());
    EXPECT_EQ(a.get(), "value of a");
    const B b(values);
    EXPECT_TRUE(b.initialized());
    EXPECT_EQ(b.get(), "value of b");
}

TEST(ParameterTest, get_value_from_parameter_should_copy_value) {
    const A a("value of a");
    EXPECT_EQ(a.get(), "value of a");
    EXPECT_EQ(a.get(), "value of a");
}

TEST(ParameterTest, take_value_from_parameter_should_move_value_parameter_becomes_uninitialized) {
    A a("value of a");
    const auto value = a.take();
    EXPECT_EQ(value, "value of a");
    EXPECT_FALSE(a.initialized());
    EXPECT_EQ(a.get(), std::string());
}

} // namespace
