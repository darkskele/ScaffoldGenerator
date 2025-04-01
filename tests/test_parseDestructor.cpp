#include "SpecialMemberFunctionParser.h"
#include "ScaffoldModels.h"
#include <gtest/gtest.h>

using namespace SpecialMemberFunctionParser;
using namespace ScaffoldModels;

TEST(DestructorParserTest, ParsesWithDescription) {
    std::vector<std::string_view> lines = {
        "| description = \"Cleans up the class\""
    };

    auto dtor = parseDestructorProperties(lines);
    EXPECT_EQ(dtor.description, "Cleans up the class");
}

TEST(DestructorParserTest, ParsesWithNoDescription) {
    auto dtor = parseDestructorProperties({});
    EXPECT_TRUE(dtor.description.empty());
}

TEST(DestructorParserTest, TrimsAndUnquotesDescription) {
    std::vector<std::string_view> lines = {
        "| description = \"   Properly releases memory and resources   \""
    };

    auto dtor = parseDestructorProperties(lines);
    EXPECT_EQ(dtor.description, "Properly releases memory and resources");
}

TEST(DestructorParserTest, ThrowsOnUnknownProperty) {
    std::vector<std::string_view> lines = {
        "| notvalid = value"
    };

    EXPECT_THROW({
        parseDestructorProperties(lines);
    }, std::runtime_error);
}
