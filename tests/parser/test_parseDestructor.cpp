#include "SpecialMemberFunctionParser.h"
#include "ClassModels.h"
#include <gtest/gtest.h>
#include <deque>
#include <string_view>

using namespace SpecialMemberFunctionParser;
using namespace ClassModels;

TEST(DestructorParserTest, ParsesWithDescription) {
    std::deque<std::string_view> lines = {
        "| description = \"Cleans up the class\"",
        "_"  // End-of-block marker (optional if no further content is expected)
    };

    auto dtor = parseDestructorProperties(lines);
    EXPECT_EQ(dtor.description, "Cleans up the class");
}

TEST(DestructorParserTest, ParsesWithNoDescription) {
    std::deque<std::string_view> lines;  // Empty input deque
    auto dtor = parseDestructorProperties(lines);
    EXPECT_TRUE(dtor.description.empty());
}

TEST(DestructorParserTest, TrimsAndUnquotesDescription) {
    std::deque<std::string_view> lines = {
        "| description = \"   Properly releases memory and resources   \"",
        "_"  // End-of-block marker
    };

    auto dtor = parseDestructorProperties(lines);
    EXPECT_EQ(dtor.description, "Properly releases memory and resources");
}

TEST(DestructorParserTest, ThrowsOnUnknownProperty) {
    std::deque<std::string_view> lines = {
        "| notvalid = value",
        "_"  // End-of-block marker
    };

    EXPECT_THROW({
        parseDestructorProperties(lines);
    }, std::runtime_error);
}
