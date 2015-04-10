#include <map>

#include <boost/algorithm/string/predicate.hpp>

#include <gtest/gtest.h>

#include <yamail/yextractor.hpp>

namespace {

using namespace testing;

using namespace yamail::yextractor;
using namespace yamail::yextractor::detail;

using Source = std::map<std::string, std::string>;

struct MdbParser {
    Errors operator ()(std::string& dst, const std::string& src) const {
        if (!boost::starts_with(src, "mdb")) {
            return Errors("value does't not starts with 'mdb'");
        }
        dst = src;
        return Errors();
    }
};

DEFINE_PARAMETER(Uid, std::string, "uid")
DEFINE_PARAMETER(Suid, std::string, "suid")
DEFINE_PARAMETER_WITH_PARSER(Mdb, std::string, "mdb", MdbParser)
DEFINE_PARAMETER(SettingsList, std::string, "settings_list")
DEFINE_PARAMETER(AskValidator, bool, "ask_validator")

static_assert(std::is_same<
                std::tuple<>,
                decltype(flat(Set<>()))
              >::value, "flat(Set<>()) doesn't returns std::tuple<>");

static_assert(std::is_same<
                std::tuple<Uid>,
                decltype(flat(Set<Required<Uid>>()))
              >::value, "flat(Set<Required<Uid>>()) doesn't returns std::tuple<Uid>");
static_assert(std::is_same<
                std::tuple<Uid, Suid>,
                decltype(flat(Set<Required<Uid>, Required<Suid>>()))
              >::value, "flat(Set<Required<Uid>, Required<Suid>>()) doesn't returns std::tuple<Uid, Suid>");

static_assert(std::is_same<
                std::tuple<Uid, Suid>,
                decltype(flat(Set<Every<Uid, Suid>>()))
              >::value, "flat(Set<Every<Uid, Suid>>()) doesn't returns std::tuple<Uid, Suid>");
static_assert(std::is_same<
                std::tuple<Uid, Suid>,
                decltype(flat(Set<Any<Uid, Suid>>()))
              >::value, "flat(Set<Any<Uid, Suid>>()) doesn't returns std::tuple<Uid, Suid>");

static_assert(std::is_same<
                std::tuple<Uid, Suid>,
                decltype(flat(Set<Every<Uid, Suid>>()))
              >::value, "flat(Set<Every<Uid, Suid>>()) doesn't returns std::tuple<Uid, Suid>");
static_assert(std::is_same<
                std::tuple<Uid, Suid>,
                decltype(flat(Set<Any<Uid, Suid>>()))
              >::value, "flat(Set<Any<Uid, Suid>>()) doesn't returns std::tuple<Uid, Suid>");

static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb, AskValidator>,
                decltype(flat(Set<Any<Uid, Suid>, Any<Mdb, AskValidator>>()))
              >::value, "flat(Set<Any<Uid, Suid>, Any<Mdb, AskValidator>>()) "
                        "doesn't returns std::tuple<Uid, Suid, Mdb, AskValidator>");
static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb, AskValidator>,
                decltype(flat(Set<Any<Uid, Suid>, Every<Mdb, AskValidator>>()))
              >::value, "flat(Set<Any<Uid, Suid>, Every<Mdb, AskValidator>>()) "
                        "doesn't returns std::tuple<Uid, Suid, Mdb, AskValidator>");
static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb, AskValidator>,
                decltype(flat(Set<Every<Uid, Suid>, Any<Mdb, AskValidator>>()))
              >::value, "flat(Set<EveryUid, Suid>, Any<Mdb, AskValidator>>()) "
                        "doesn't returns std::tuple<Uid, Suid, Mdb, AskValidator>");
static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb, AskValidator>,
                decltype(flat(Set<Every<Uid, Suid>, Every<Mdb, AskValidator>>()))
              >::value, "flat(Set<Every<Uid, Suid>, Every<Mdb, AskValidator>>()) "
                        "doesn't returns std::tuple<Uid, Suid, Mdb, AskValidator>");

static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb>,
                decltype(flat(Set<Any<Uid, Any<Suid, Mdb>>>()))
              >::value, "flat(Set<Any<Uid, Any<Suid, Mdb>>>()) doesn't returns std::tuple<Uid, Suid, Mdb>");
static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb>,
                decltype(flat(Set<Any<Uid, Every<Suid, Mdb>>>()))
              >::value, "flat(Set<Any<Uid, Every<Suid, Mdb>>>()) doesn't returns std::tuple<Uid, Suid, Mdb>");
static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb>,
                decltype(flat(Set<Every<Uid, Any<Suid, Mdb>>>()))
              >::value, "flat(Set<Every<Uid, Any<Suid, Mdb>>>()) doesn't returns std::tuple<Uid, Suid, Mdb>");
static_assert(std::is_same<
                std::tuple<Uid, Suid, Mdb>,
                decltype(flat(Set<Every<Uid, Every<Suid, Mdb>>>()))
              >::value, "flat(Set<Every<Uid, Every<Suid, Mdb>>>()) doesn't returns std::tuple<Uid, Suid, Mdb>");

struct ExtractorTest : Test {};

TEST(ExtractorTest, extractEmptyShouldReturnError) {
    Extractor extractor;
    EXPECT_EQ(extractor.get<Set<>>(Source()), std::tuple<>());
    EXPECT_FALSE(extractor.errors().empty());
    EXPECT_EQ(extractor.errors(), Errors("no values"));
}

TEST(ExtractorTest, extractUidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Uid>>(Source())),
            std::tuple<Uid>
        >::value,
        "Extractor().get<Set<Uid>>(Source()) doesn't returns std::tuple<Uid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}};
    EXPECT_EQ(extractor.get<Set<Uid>>(source), std::make_tuple(Uid("42")));
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractRequiredUidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Required<Uid>>>(Source())),
            std::tuple<Uid>
        >::value,
        "Extractor().get<Set<Required<Uid>>>(Source()) doesn't returns std::tuple<Uid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}};
    EXPECT_EQ(extractor.get<Set<Required<Uid>>>(source), std::make_tuple(Uid("42")));
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractAbsentRequiredUidShouldReturnError) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Required<Uid>>>(Source())),
            std::tuple<Uid>
        >::value,
        "Extractor().get<Set<Required<Uid>>>(Source()) doesn't returns std::tuple<Uid>");
    Extractor extractor;
    EXPECT_EQ(extractor.get<Set<Required<Uid>>>(Source()), std::make_tuple(Uid()));
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors({"parameter 'uid' is required", "parameter 'uid' not found"});
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extractOptionalUidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Optional<Uid>>>(Source())),
            std::tuple<Uid>
        >::value,
        "Extractor().get<Set<Optional<Uid>>>(Source()) doesn't returns std::tuple<Uid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}};
    EXPECT_EQ(extractor.get<Set<Optional<Uid>>>(source), std::make_tuple(Uid("42")));
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractAbsentOptionalUidShouldReturnError) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Optional<Uid>>>(Source())),
            std::tuple<Uid>
        >::value,
        "Extractor().get<Set<Optional<Uid>>>(Source()) doesn't returns std::tuple<Uid>");
    Extractor extractor;
    EXPECT_EQ(extractor.get<Set<Optional<Uid>>>(Source()), std::make_tuple(Uid()));
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractUidAndSuidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Uid, Suid>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Uid, Suid>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}, {"suid", "13"}};
    const auto real = extractor.get<Set<Uid, Suid>>(source);
    const auto expected = std::make_tuple(Uid("42"), Suid("13"));
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractMdbWithCustomParserShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Mdb>>(Source())),
            std::tuple<Mdb>
        >::value,
        "Extractor().get<Set<Mdb>>(Source()) doesn't returns std::tuple<Mdb>");
    Extractor extractor;
    const Source source = {{"mdb", "mdb042"}};
    const auto real = extractor.get<Set<Mdb>>(source);
    const auto expected = std::make_tuple(Mdb("mdb042"));
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractWrongMdbWithCustomParserShouldReturnError) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Mdb>>(Source())),
            std::tuple<Mdb>
        >::value,
        "Extractor().get<Set<Mdb>>(Source()) doesn't returns std::tuple<Mdb>");
    Extractor extractor;
    const Source source = {{"mdb", "042"}};
    const auto real = extractor.get<Set<Mdb>>(source);
    const auto expected = std::make_tuple(Mdb());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    EXPECT_EQ(extractor.errors(), Errors("value does't not starts with 'mdb'"));
}

TEST(ExtractorTest, extractWrongRequiredMdbWithCustomParserShouldReturnErrors) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Required<Mdb>>>(Source())),
            std::tuple<Mdb>
        >::value,
        "Extractor().get<Set<Required<Mdb>>>(Source()) doesn't returns std::tuple<Mdb>");
    Extractor extractor;
    const Source source = {{"mdb", "042"}};
    const auto real = extractor.get<Set<Required<Mdb>>>(source);
    const auto expected = std::make_tuple(Mdb());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {"parameter 'mdb' is required", "value does't not starts with 'mdb'"};
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extractWrongOptionalMdbWithCustomParserShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Optional<Mdb>>>(Source())),
            std::tuple<Mdb>
        >::value,
        "Extractor().get<Set<Optional<Mdb>>>(Source()) doesn't returns std::tuple<Mdb>");
    Extractor extractor;
    const Source source = {{"mdb", "042"}};
    const auto real = extractor.get<Set<Optional<Mdb>>>(source);
    const auto expected = std::make_tuple(Mdb());
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractEveryOfUidAndSuidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Every<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Every<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}, {"suid", "13"}};
    const auto real = extractor.get<Set<Every<Uid, Suid>>>(source);
    const auto expected = std::make_tuple(Uid("42"), Suid("13"));
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractEveryOfUidAndSuidWithoutUidShouldReturnErrors) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Every<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Every<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"suid", "13"}};
    const auto real = extractor.get<Set<Every<Uid, Suid>>>(source);
    const auto expected = std::make_tuple(Uid(), Suid("13"));
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not every of parameters ('uid' and 'suid') are present",
        "parameter 'uid' not found"
   };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extractEveryOfUidAndSuidWithoutSuidShouldReturnErrors) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Every<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Every<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}};
    const auto real = extractor.get<Set<Every<Uid, Suid>>>(source);
    const auto expected = std::make_tuple(Uid("42"), Suid());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not every of parameters ('uid' and 'suid') are present",
        "parameter 'suid' not found"
   };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extractEveryOfUidAndSuidWithoutAllShouldReturnErrors) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Every<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Every<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const auto real = extractor.get<Set<Every<Uid, Suid>>>(Source());
    const auto expected = std::make_tuple(Uid(), Suid());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not every of parameters ('uid' and 'suid') are present",
        "parameter 'uid' not found",
        "parameter 'suid' not found"
    };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extractAnyOfUidAndSuidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Any<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Any<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}, {"suid", "13"}};
    const auto real = extractor.get<Set<Any<Uid, Suid>>>(source);
    const auto expected = std::make_tuple(Uid("42"), Suid("13"));
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractAnyOfUidAndSuidWithoutUidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Any<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Any<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"suid", "13"}};
    const auto real = extractor.get<Set<Any<Uid, Suid>>>(source);
    const auto expected = std::make_tuple(Uid(), Suid("13"));
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractAnyOfUidAndSuidWithoutUidAndSuidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<Any<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<Any<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const auto real = extractor.get<Set<Any<Uid, Suid>>>(Source());
    const auto expected = std::make_tuple(Uid(), Suid());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not any of parameters ('uid' or 'suid') are present",
        "parameter 'uid' not found",
        "parameter 'suid' not found"
    };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extractFirstOfUidAndSuidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<First<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<First<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"uid", "42"}, {"suid", "13"}};
    const auto real = extractor.get<Set<First<Uid, Suid>>>(source);
    const auto expected = std::make_tuple(Uid("42"), Suid());
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractFirstOfUidAndSuidWithoutUidShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<First<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<First<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const Source source = {{"suid", "13"}};
    const auto real = extractor.get<Set<First<Uid, Suid>>>(source);
    const auto expected = std::make_tuple(Uid(), Suid("13"));
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

TEST(ExtractorTest, extractFirstOfUidAndSuidWithoutAllShouldReturnErrors) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<Set<First<Uid, Suid>>>(Source())),
            std::tuple<Uid, Suid>
        >::value,
        "Extractor().get<Set<First<Uid, Suid>>>(Source()) doesn't returns std::tuple<Uid, Suid>");
    Extractor extractor;
    const auto real = extractor.get<Set<First<Uid, Suid>>>(Source());
    const auto expected = std::make_tuple(Uid(), Suid());
    EXPECT_EQ(real, expected);
    EXPECT_FALSE(extractor.errors().empty());
    const Errors errors = {
        "not any of parameters ('uid' or 'suid') are present when looking for first",
        "parameter 'uid' not found",
        "parameter 'suid' not found"
    };
    EXPECT_EQ(extractor.errors(), errors);
}

TEST(ExtractorTest, extractComplexSetShouldSucceed) {
    static_assert(
        std::is_same<
            decltype(Extractor().get<
                Set<
                    Any<Uid, Every<Suid, Mdb>>,
                    Required<SettingsList>,
                    Optional<AskValidator>
                >>(Source())),
            std::tuple<Uid, Suid, Mdb, SettingsList, AskValidator>
        >::value,
        "Extractor().get<\n"
            "Set<\n"
                "Any<Uid, Every<Suid, Mdb>>,\n"
                "Required<SettingsList>,\n"
                "Optional<AskValidator>\n"
            ">(Source()) doesn't returns std::tuple<Uid, Suid, Mdb, SettingsList, AskValidator>");
    Extractor extractor;
    const Source source = {
        {"uid", "42"},
        {"suid", "13"},
        {"mdb", "mdb042"},
        {"settings_list", "from_name"},
        {"ask_validator", "y"},
    };
    using Params = Set<
        Any<Uid, Every<Suid, Mdb>>,
        Required<SettingsList>,
        Optional<AskValidator>
    >;
    const auto real = extractor.get<Params>(source);
    const auto expected = std::make_tuple(Uid("42"), Suid("13"), Mdb("mdb042"),
                                          SettingsList("from_name"), AskValidator(true));
    EXPECT_EQ(real, expected);
    EXPECT_TRUE(extractor.errors().empty());
}

} // namespace
