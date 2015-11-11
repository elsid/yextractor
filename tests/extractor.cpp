#include <map>

#include <boost/algorithm/string/predicate.hpp>

#include <gtest/gtest.h>

#include <yamail/yextractor.hpp>
#include <yamail/yextractor/expression.hpp>

#include "tests.hpp"

namespace {

using namespace testing;
using namespace tests;

using namespace yamail::yextractor;
using namespace yamail::yextractor::detail;

using Source = std::map<std::string, std::string>;

struct ParamParser {
    Errors operator ()(std::string& dst, const std::string& src) const {
        if (!boost::starts_with(src, "prefix")) {
            return Errors("value does't not starts with 'prefix'");
        }
        dst = src;
        return Errors();
    }
};

DEFINE_PARAMETER(ParamA, std::string, "a")
DEFINE_PARAMETER(ParamB, std::string, "b")
DEFINE_PARAMETER_WITH_PARSER(ParamWithParser, std::string, "c", ParamParser)
DEFINE_PARAMETER(ParamD, std::string, "d")
DEFINE_PARAMETER(ParamE, std::string, "e")

static_assert(
    std::is_same<
        Expression<Required<ParamA>>::Type,
        std::tuple<ParamA>
    >::value,
    "Extractor().get<Required<ParamA>>(Source()) doesn't returns std::tuple<ParamA>");

static_assert(
    std::is_same<
        Expression<Optional<ParamA>>::Type,
        std::tuple<ParamA>
    >::value,
    "Extractor().get<Optional<ParamA>>(Source()) doesn't returns std::tuple<ParamA>");

static_assert(
    std::is_same<
        Expression<Every<ParamA, ParamB>>::Type,
        std::tuple<ParamA, ParamB>
    >::value,
    "Extractor().get<Every<ParamA, ParamB>>(Source()) doesn't returns std::tuple<ParamA, ParamB>");

static_assert(
    std::is_same<
        Expression<Every<Optional<ParamA>, Optional<ParamB>>>::Type,
        std::tuple<ParamA, ParamB>
    >::value,
    "Extractor().get<Every<Optional<ParamA>, Optional<ParamB>>>(Source()) doesn't returns std::tuple<ParamA, ParamB>");

static_assert(
    std::is_same<
        Expression<Every<ParamA, ParamB>>::Type,
        std::tuple<ParamA, ParamB>
    >::value,
    "Extractor().get<Every<ParamA, ParamB>>(Source()) doesn't returns std::tuple<ParamA, ParamB>");

static_assert(
    std::is_same<
        Expression<Any<ParamA, ParamB>>::Type,
        std::tuple<ParamA, ParamB>
    >::value,
    "Extractor().get<Any<ParamA, ParamB>>(Source()) doesn't returns std::tuple<ParamA, ParamB>");

static_assert(
    std::is_same<
        Expression<First<ParamA, ParamB>>::Type,
        std::tuple<ParamA, ParamB>
    >::value,
    "Extractor().get<First<ParamA, ParamB>>(Source()) doesn't returns std::tuple<ParamA, ParamB>");

static_assert(
    std::is_same<
        Expression<
            Every<
                Any<ParamA, Every<ParamB, ParamWithParser>>,
                Required<ParamD>,
                Optional<ParamE>
            >>::Type,
        std::tuple<ParamA, ParamB, ParamWithParser, ParamD, ParamE>
    >::value,
    "Extractor().get<\n"
        "Every<\n"
            "Any<ParamA, Every<ParamB, ParamWithParser>>,\n"
            "Required<ParamD>,\n"
            "Optional<ParamE>\n"
        ">(Source()) doesn't returns std::tuple<ParamA, ParamB, ParamWithParser, ParamD, ParamE>");

struct ExtractorTest : Test {};

TEST(ExtractorTest, extract_from_empty_every_should_return_errors) {
    Extractor extractor;
    EXPECT_EQ(extractor.get<Every<>>(Source()), std::tuple<>());
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors({"not every of parameters () are present", "no values"});
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_every_of_one_param_should_succeed) {
    Extractor extractor;
    const Source source = {{"a", "42"}};
    EXPECT_EQ(extractor.get<Every<ParamA>>(source), std::make_tuple(ParamA("42")));
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_required_param_should_succeed) {
    Extractor extractor;
    const Source source = {{"a", "42"}};
    EXPECT_EQ(extractor.get<Required<ParamA>>(source), std::make_tuple(ParamA("42")));
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_required_param_from_empty_source_return_error) {
    Extractor extractor;
    EXPECT_EQ(extractor.get<Required<ParamA>>(Source()), std::make_tuple(ParamA()));
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors({"parameter 'a' is required", "parameter 'a' not found"});
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_optional_param_should_succeed) {
    Extractor extractor;
    const Source source = {{"a", "42"}};
    EXPECT_EQ(extractor.get<Optional<ParamA>>(source), std::make_tuple(ParamA("42")));
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_optional_param_from_empty_source_should_return_succeed) {
    Extractor extractor;
    EXPECT_EQ(extractor.get<Optional<ParamA>>(Source()), std::make_tuple(ParamA()));
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_two_params_from_set_of_two_params_should_succeed) {
    Extractor extractor;
    const Source source = {{"a", "42"}, {"b", "13"}};
    const auto real = extractor.get<Every<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA("42"), ParamB("13"));
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_required_param_with_custom_parser_should_succeed) {
    Extractor extractor;
    const Source source = {{"c", "prefix042"}};
    const auto real = extractor.get<Required<ParamWithParser>>(source);
    const auto expected = std::make_tuple(ParamWithParser("prefix042"));
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_required_param_with_custom_parser_that_value_is_wrong_should_return_errors) {
    Extractor extractor;
    const Source source = {{"c", "042"}};
    const auto real = extractor.get<Required<ParamWithParser>>(source);
    const auto expected = std::make_tuple(ParamWithParser());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {"parameter 'c' is required", "value does't not starts with 'prefix'"};
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_optional_param_with_custom_parser_that_value_is_wrong_should_succeed) {
    Extractor extractor;
    const Source source = {{"c", "042"}};
    const auto real = extractor.get<Optional<ParamWithParser>>(source);
    const auto expected = std::make_tuple(ParamWithParser());
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_every_of_two_params_should_succeed) {
    Extractor extractor;
    const Source source = {{"a", "42"}, {"b", "13"}};
    const auto real = extractor.get<Every<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA("42"), ParamB("13"));
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_every_of_two_params_from_source_without_first_param_value_should_return_errors) {
    Extractor extractor;
    const Source source = {{"b", "13"}};
    const auto real = extractor.get<Every<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA(), ParamB("13"));
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not every of parameters ('a' and 'b') are present",
        "parameter 'a' not found"
   };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_every_of_two_params_from_source_without_second_param_value_should_return_errors) {
    Extractor extractor;
    const Source source = {{"a", "42"}};
    const auto real = extractor.get<Every<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA("42"), ParamB());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not every of parameters ('a' and 'b') are present",
        "parameter 'b' not found"
   };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_every_of_two_params_from_empty_source_should_return_errors) {
    Extractor extractor;
    const auto real = extractor.get<Every<ParamA, ParamB>>(Source());
    const auto expected = std::make_tuple(ParamA(), ParamB());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not every of parameters ('a' and 'b') are present",
        "parameter 'a' not found",
        "parameter 'b' not found"
    };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_any_of_two_params_should_succeed) {
    Extractor extractor;
    const Source source = {{"a", "42"}, {"b", "13"}};
    const auto real = extractor.get<Any<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA("42"), ParamB("13"));
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_any_of_two_params_from_source_without_one_param_value_should_succeed) {
    Extractor extractor;
    const Source source = {{"b", "13"}};
    const auto real = extractor.get<Any<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA(), ParamB("13"));
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_any_of_two_params_from_empty_source_should_return_errors) {
    Extractor extractor;
    const auto real = extractor.get<Any<ParamA, ParamB>>(Source());
    const auto expected = std::make_tuple(ParamA(), ParamB());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not any of parameters ('a' or 'b') are present",
        "parameter 'a' not found",
        "parameter 'b' not found"
    };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_first_of_two_params_should_succeed) {
    Extractor extractor;
    const Source source = {{"a", "42"}, {"b", "13"}};
    const auto real = extractor.get<First<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA("42"), ParamB());
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_first_of_two_params_from_source_without_one_param_should_succeed) {
    Extractor extractor;
    const Source source = {{"b", "13"}};
    const auto real = extractor.get<First<ParamA, ParamB>>(source);
    const auto expected = std::make_tuple(ParamA(), ParamB("13"));
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

TEST(ExtractorTest, extract_first_of_two_params_from_empty_source_should_return_errors) {
    Extractor extractor;
    const auto real = extractor.get<First<ParamA, ParamB>>(Source());
    const auto expected = std::make_tuple(ParamA(), ParamB());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not any of parameters ('a' or 'b') are present when looking for first",
        "parameter 'a' not found",
        "parameter 'b' not found"
    };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extract_complex_expression_should_succeed) {
    Extractor extractor;
    const Source source = {
        {"a", "42"},
        {"b", "13"},
        {"c", "prefix042"},
        {"d", "str"},
        {"e", "y"},
    };
    using Params = Every<
        Any<ParamA, Every<ParamB, ParamWithParser>>,
        Required<ParamD>,
        Optional<ParamE>
    >;
    const auto real = extractor.get<Params>(source);
    const auto expected = std::make_tuple(ParamA("42"), ParamB("13"),
                                          ParamWithParser("prefix042"),
                                          ParamD("str"), ParamE("y"));
    EXPECT_EQ(real, expected);
    EXPECT_EQ(extractor.errors(), Errors());
}

} // namespace
