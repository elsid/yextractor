#include <map>

#include <boost/algorithm/string.hpp>

#include <yamail/yextractor.hpp>
#include <yamail/yextractor/expression.hpp>

using namespace yamail::yextractor;

struct MdbParser {
    Errors operator ()(std::string &dst, const std::string &src) const {
        if (!boost::starts_with(src, "mdb")) {
            return Errors("value of 'mdb' parameter does't not starts with 'mdb'");
        }
        dst = src;
        return Errors();
    }
};

struct SettingsListParser {
    Errors operator ()(std::vector<std::string> &dst, const std::string &src) const {
        if (src.empty()) {
            return Errors("no values in 'settings_list' parameter");
        }
        boost::split(dst, src, boost::is_any_of("\r"), boost::token_compress_on);
        if (dst.empty() || (dst.size() == 1 && dst.front() == "")) {
            return Errors("empty values in 'settings_list' parameter");
        }
        return Errors();
    }
};

struct FlagParser {
    Errors operator ()(bool &dst, const std::string &src) const {
        dst = src == "y";
        return Errors();
    }
};

DEFINE_PARAMETER(Uid, std::string, "uid")
DEFINE_PARAMETER(Suid, std::string, "suid")
DEFINE_PARAMETER_WITH_PARSER(Mdb, std::string, "mdb", MdbParser)
DEFINE_PARAMETER_WITH_PARSER(SettingsList, std::vector<std::string>, "settings_list",
                             SettingsListParser)
DEFINE_PARAMETER_WITH_PARSER(AskValidator, bool, "ask_validator", FlagParser)

using GetParameters = Every<
    Any<Uid, Every<Suid, Mdb>>,
    SettingsList,
    Optional<AskValidator>
>;

void printValues(const Expression<GetParameters>::Type& values) {
    const Uid uid(values);
    const Suid suid(values);
    const Mdb mdb(values);
    const SettingsList settingsList(values);
    const AskValidator askValidator(values);

    std::cout << "\nValues:" << std::endl;
    std::cout << Uid::name() << ": " << uid.get() << std::endl;
    std::cout << Suid::name() << ": " << suid.get() << std::endl;
    std::cout << Mdb::name() << ": " << mdb.get() << std::endl;
    std::cout << SettingsList::name() << ": " << boost::join(settingsList.get(), ", ") << std::endl;
    std::cout << AskValidator::name() << ": " << std::boolalpha << askValidator.get() << std::endl;
}

void printErrors(const Errors& errors) {
    std::cout << "\nErrors: " << std::endl;
    std::cout << boost::join(errors.messages(), "\n") << std::endl;
}

int main(int, char *[]) {
    using Source = std::map<std::string, std::string>;

    const Source source = {
        {"uid", "42"},
        {"suid", "13"},
        {"mdb", "mdb666"},
        {"settings_list", "from_name\rsignature"},
        {"ask_validator", "y"},
    };

    Extractor extractor;

    std::cout << "Trying extract values from filled map..." << std::endl;

    const auto filledValues = extractor.get<GetParameters>(source);

    if (extractor.errors().empty()) {
        printValues(filledValues);
    } else {
        std::cout << "Something is wrong" << std::endl;
        printErrors(extractor.errors());
    }

    std::cout << "Trying extract values from empty map..." << std::endl;

    const auto emptyValues = extractor.get<GetParameters>(Source());

    if (extractor.errors().empty()) {
        std::cout << "I don't have any idea why it has happend..." << std::endl;
        printValues(emptyValues);
    } else {
        printErrors(extractor.errors());
    }

    const Source wrongSource = {
        {"suid", "13"},
        {"mdb", "666"},
        {"settings_list", ""},
    };

    std::cout << "Trying extract values from map with wrong values..." << std::endl;

    const auto wrongValues = extractor.get<GetParameters>(wrongSource);

    if (extractor.errors().empty()) {
        std::cout << "May be it's not really wrong..." << std::endl;
        printValues(wrongValues);
    } else {
        printErrors(extractor.errors());
    }

    return 0;
}
