#include <map>

#include <gtest/gtest.h>

#include <yamail/yextractor/parameter.hpp>
#include <yamail/yextractor/exclude.hpp>

#include "tests.hpp"

namespace {

using namespace testing;
using namespace tests;

using namespace yamail::yextractor;

DEFINE_PARAMETER(A, std::string, "a")
DEFINE_PARAMETER(B, std::string, "b")

struct ExcludeTest : Test {};

using Container = std::map<std::string, std::string>;

TEST(ExcludeTest, exclude_no_parameters_from_empty_container_should_succeed) {
    Container container;
    exclude<std::tuple<>>(container);
    EXPECT_TRUE(container.empty());
}

TEST(ExcludeTest, exclude_one_parameter_from_empty_container_should_succeed) {
    Container container;
    exclude<std::tuple<A>>(container);
    EXPECT_TRUE(container.empty());
}

TEST(ExcludeTest, exclude_two_parameters_from_empty_container_should_succeed) {
    Container container;
    exclude<std::tuple<A, B>>(container);
    EXPECT_TRUE(container.empty());
}

TEST(ExcludeTest, exclude_two_parameters_from_container_with_one_value_should_succeed) {
    Container container = {{"a", "1"}};
    exclude<std::tuple<A, B>>(container);
    EXPECT_TRUE(container.empty());
}

TEST(ExcludeTest, exclude_two_parameters_from_container_with_two_values_should_succeed) {
    Container container = {{"a", "1"}, {"b", "2"}};
    exclude<std::tuple<A, B>>(container);
    EXPECT_TRUE(container.empty());
}

TEST(ExcludeTest, exclude_one_parameter_from_container_with_two_values_should_left_one_value) {
    Container container = {{"a", "1"}, {"b", "2"}};
    exclude<std::tuple<A>>(container);
    const Container expected = {{"b", "2"}};
    EXPECT_EQ(container, expected);
}

} // namespace
